//----------------------------------------------------
//Interface for XClassTokenize
//Created automatically at 2020.09.14 11:01:46
//----------------------------------------------------
//Page Input
//Input text to process.

//Enum Source
//Source of input files.
enum enum_source {
    source_Text = 0,
    source_Text_file = 1,
    source_Var = 2,
    source_URL = 3,
    source_Script = 4,
    source_N__ = 5
};
bool was_changed_source() { return was_changed("source"); }
enum_source gete_source() { return enum_source(geti("source")); }

//Text Text
//Input area.
bool was_changed_input_text() { return was_changed("input_text"); }
QString gets_input_text() { return gets("input_text"); }
QStringList get_strings_input_text() { return get_strings("input_text"); }

//String File
//File name.
bool was_changed_input_file() { return was_changed("input_file"); }
QString gets_input_file() { return gets("input_file"); }
QStringList get_strings_input_file() { return get_strings("input_file"); }

//String Var
//Var name.
bool was_changed_input_var() { return was_changed("input_var"); }
QString gets_input_var() { return gets("input_var"); }
QStringList get_strings_input_var() { return get_strings("input_var"); }

//String URL
//URL for text to download.
bool was_changed_input_url() { return was_changed("input_url"); }
QString gets_input_url() { return gets("input_url"); }
QStringList get_strings_input_url() { return get_strings("input_url"); }

//Text Script
//Script for input.
bool was_changed_input_script() { return was_changed("input_script"); }
QString gets_input_script() { return gets("input_script"); }
QStringList get_strings_input_script() { return get_strings("input_script"); }

//----------------------------------------------------
//Page Process
//Perform html cleaning using readability-lxml.

//Checkbox HTML filter
//Filter HTML using  readability-lxml.
bool was_changed_html_filter() { return was_changed("html_filter"); }
int geti_html_filter() { return geti("html_filter"); }

//Checkbox Extract Paragraphs
//Extract paragraphs.
bool was_changed_get_paragraphs() { return was_changed("get_paragraphs"); }
int geti_get_paragraphs() { return geti("get_paragraphs"); }

//Checkbox Extract Sentences
//Extract sentences.
bool was_changed_get_sentences() { return was_changed("get_sentences"); }
int geti_get_sentences() { return geti("get_sentences"); }

//Checkbox Tokenize
//Extract tokens.
bool was_changed_tokenize() { return was_changed("tokenize"); }
int geti_tokenize() { return geti("tokenize"); }

//Checkbox Extract Lexems
//Extract lexems.
bool was_changed_get_lexems() { return was_changed("get_lexems"); }
int geti_get_lexems() { return geti("get_lexems"); }


//Enum Result
//Output type - JSON Text is good for small data, JSON Object works faster, Python Generator is useful for further processing with Python.
enum enum_result {
    result_JSON_Text = 0,
    result_JSON_Object = 1,
    result_Python_Generator = 2,
    result_N__ = 3
};
bool was_changed_result() { return was_changed("result"); }
enum_result gete_result() { return enum_result(geti("result")); }

//----------------------------------------------------
