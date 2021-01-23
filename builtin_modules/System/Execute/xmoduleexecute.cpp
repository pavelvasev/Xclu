#include "xmoduleexecute.h"
#include "incl_cpp.h"
#include "registrarxmodule.h"
#include <QProcess>
#include "xcore.h"


//registering module implementation
REGISTER_XMODULE(Execute)

//---------------------------------------------------------------------
XModuleExecute::XModuleExecute(QString class_name)
    :XModule(class_name)
{

}

//---------------------------------------------------------------------
XModuleExecute::~XModuleExecute()
{

}

//---------------------------------------------------------------------
void XModuleExecute::start() {
    //run
    sete_status(status_Not_Started);
    seti_exit_code(0);

    clear_string_error_details();

    //read, write
    console_clear();

    //debug
    seti_executed_times(0);
    clear_string_folder_path();
    clear_string_file_path();


    //process
    subprocess_.reset();

    crashed_ = false;

    //run at first frame
    if (gete_execute_event() == execute_event_At_First_Frame) {
        process_run();
    }
}

//---------------------------------------------------------------------
void XModuleExecute::update() {
    bool need_run = false;
    //buttons
    if (geti_run_button()) {
        need_run = true;
    }
    if (geti_stop_button()) {
        process_stop();
    }

    //events
    auto execute_event = gete_execute_event();
    if (execute_event == execute_event_Each_Frame) {
        need_run = true;
    }

    //run if required
    if (need_run) {
        process_run();
    }

    //read console if required
    if (gete_read() == read_Each_Frame) {
        console_read();
    }

}

//---------------------------------------------------------------------
void XModuleExecute::stop() {
    subprocess_.reset();
}

//---------------------------------------------------------------------
void XModuleExecute::process_stop() {
    if (subprocess_.data()) {
        if (gete_status() == status_Running) {
            subprocess_->terminate();
            //on_finish(); //TODO I think it's not needed
        }
    }
}

//---------------------------------------------------------------------
void XModuleExecute::console_clear() {
    clear_string_console_errors_text();
    clear_string_console_read_string();
    clear_string_console_read_text();

}

//---------------------------------------------------------------------
void XModuleExecute::process_run() {
    if (gete_status() == status_Running)  {
        return;
    }

    //clear console
    console_clear();

    //compute paths
    QString folder = gets_folder_name();
    QString file_name_short = gets_file_name();

    folder = xc_project_folder() + "/" + folder;
    sets_folder_path(folder);

    //file name
    xc_assert(!file_name_short.isEmpty(), "File Name is empty");
    QString file_name = folder + "/" + file_name_short;
    //if user provided absolute path - then use it - we detect it as if file_name doesn't extsts
    if (!QFileInfo::exists(file_name)) {
        file_name = file_name_short;
    }
    sets_file_path(file_name);

    //check that folder and file exist
    xc_assert(QDir(folder).exists(), "Folder '" + folder + "' doesn't exists");
    xc_assert(QFileInfo::exists(file_name), "File '" + file_name + "' doesn't exists");

    //starting time
    //double start_time = xc_elapsed_time_sec();
    //setf_last_time(start_time);

    //qDebug() << "execute...";

    //create process
    //Note: this stops current process if exists - don't know, should we manage this additionally
    subprocess_.reset(new QProcess);

    QProcess &subprocess = *subprocess_;
    subprocess.setWorkingDirectory(folder);
    subprocess.setProgram(file_name);

    //arguments
    QStringList arguments;
    arguments << gets_args();
    subprocess.setArguments(arguments);

    //variables
    //timeout - if -1, then wait infinite
    int timeout = (geti_enable_timeout()) ? int(getf_timeout_sec()*1000): -1;

    crashed_ = false;

    //connect events listeners
    subprocess.setReadChannel(QProcess::StandardOutput);
    subprocess.setProcessChannelMode(QProcess::SeparateChannels); //separate output and error to process them differently
    //subprocess.setProcessChannelMode(QProcess::MergedChannels);

    connect(&subprocess, QOverload<QProcess::ProcessError>::of(&QProcess::error), this, &XModuleExecute::crashed);

    //start process
    sete_status(status_Running);
    subprocess.start(QProcess::Unbuffered | QProcess::ReadWrite );

    //increase counter
    increase_int_executed_times();

    //decide - wait finishing or work async
    auto thread_mode = gete_thread_mode();

    if (thread_mode == thread_mode_Wait_Finishing) {
        //sync run
        if (gete_write() == write_After_Finished) {
            console_write();
        }
        bool no_timeout = subprocess.waitForFinished(timeout);
        int exit_code = subprocess.exitCode();
        auto exit_status = subprocess.exitStatus();

        //process finish code and also read console
        on_finish(exit_code, exit_status, !no_timeout);
    }
    else {
        //async run
        //implement async events
        //connect(&subprocess, &QProcess::readyReadStandardOutput, this, &XModuleExecute::onReadyReadStandardOutput);
        connect(&subprocess, &QProcess::readyReadStandardError, this, &XModuleExecute::onReadyReadStandardError);

        connect(&subprocess, QOverload<int,QProcess::ExitStatus>::of(&QProcess::finished), this, &XModuleExecute::finished);
    }

    //double time = xc_elapsed_time_sec();
    //setf_last_duration(time - start_time);
}

//---------------------------------------------------------------------
void XModuleExecute::on_finish(int exit_code, QProcess::ExitStatus exit_status, bool timeout) {
    if (subprocess_.data()) {
        if (gete_status() == status_Running) {
            //read console
            if (gete_read() != read_Disabled) {
                console_read();
            }

            //read console error
            console_read_error();

            //exit code
            seti_exit_code(exit_code);

            //exit status
            bool success = (exit_status == QProcess::NormalExit) && (exit_code == 0);
            enum_status status = (timeout) ? status_Timeout : (success?status_Success:status_Error);
            sete_status(status);

            QString error_details;
            if (!timeout && crashed_) {
                error_details = crash_text_;
                status = status_Crashed;
            }
            sets_error_details(error_details);


            //delete process
            subprocess_.reset();

            //error handling
            if (status == status_Timeout) xc_exception("Timeout");
            if (status == status_Error) xc_exception("Execution error");

        }
    }
}

//---------------------------------------------------------------------
void XModuleExecute::onReadyReadStandardError() {
    //Note, this is can be async callback
    console_read_error();
}

//---------------------------------------------------------------------
//void XModuleExecute::onReadyReadStandardOutput() {
    //Note, this is async callback

//}

//---------------------------------------------------------------------
void XModuleExecute::finished(int exit_code, QProcess::ExitStatus exit_status) {
    //Note, this is can be async callback
    on_finish(exit_code, exit_status, false);
}

//---------------------------------------------------------------------
void XModuleExecute::crashed(QProcess::ProcessError error) {
    //Note, this is can be async callback

    //async process - inform main thread:
    crashed_ = true;

    switch (error) {
    case QProcess::FailedToStart:  crash_text_ = "File not found, resource error"; break;
    case QProcess::Crashed: crash_text_ = "Crashed"; break;
    case QProcess::Timedout: crash_text_ = "Timedout"; break;
    case QProcess::ReadError: crash_text_ = "ReadError"; break;
    case QProcess::WriteError: crash_text_ = "WriteError"; break;
    case QProcess::UnknownError: crash_text_ = "UnknownError"; break;
    default: crash_text_ = "Unknown reason";
    }

    crash_text_ = "Process crashed, the reason: " + crash_text_;

    //xc_exception("Execute Crashed");
}


//---------------------------------------------------------------------
void XModuleExecute::console_read_error() {
    QProcess *subprocess = subprocess_.data();
    if (subprocess) {
        auto console_errors = gete_console_errors();
        if (console_errors != console_errors_Ignore) {
            QString text = subprocess->readAllStandardError();
            if (!text.isEmpty()) {
                append_string_console_errors_text(text);

                //stop process if required
                if (console_errors == console_errors_Show_And_Stop) {
                    crashed_ = true;
                    crash_text_ = "Process printed something to Error Console";
                }
            }
        }
    }
}

//---------------------------------------------------------------------
void XModuleExecute::console_read() {
    QProcess *subprocess = subprocess_.data();
    if (subprocess) {
        auto data = subprocess->readAllStandardOutput();
        if (!data.isEmpty()) {
            auto read_type = gete_read_type();

            if (read_type == read_type_String) {
                sets_console_read_string(data);
            }
            if (read_type == read_type_Text) {
                sets_console_read_text(data);
            }
            if (read_type == read_type_Image) {
                //TODO
            }
        }
    }

}

//---------------------------------------------------------------------
void XModuleExecute::console_write() {
    if (gete_status() != status_Running) {
        return;
    }

    QProcess *subprocess = subprocess_.data();
    if (subprocess) {
        auto write_type = gete_write_type();
        if (write_type == write_type_String) {
            QString str = gets_console_write_string();
            auto ts = gete_line_term(); //None,\n,\r,\r\n
            if (ts == line_term__n) str += "\n";
            if (ts == line_term__r) str += "\r";
            if (ts == line_term__r_n) str += "\r\n";
            subprocess->write(str.toLatin1());
        }
        if (write_type == write_type_Text) {
            subprocess->write(gets_console_write_text().toLatin1());
        }
        if (write_type == write_type_Image) {
            //TODO
            //console_write_image
        }
    }

}

//---------------------------------------------------------------------
