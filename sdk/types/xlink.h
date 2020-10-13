#ifndef XLINK_H
#define XLINK_H

//Working with с links - creating and parsing links as following strings:
//webcam1->image     - link to component
//#module1->var      - disabled link, also empty string means disabled link too
//module1->line(1)   - link to part of component (word or line) pointed by one index
//module1->line(1,2) - link to part of component (word) pointed by two indices index

#include "incl_h.h"

//Storing link string and enabled/disabled property
class XLink {
public:
    bool enabled = false;
    QString link;
    XLink() {}
    XLink(bool enabled, QString link) {
        this->enabled = enabled;
        this->link = link;
    }
    XLink(QString link_str) {
        if (!link_str.isEmpty()) {
            if (link_str.left(1) == "#") {
                link_str.remove(0, 1);
                enabled = false;
                this->link = link_str;
            }
            else {
                enabled = true;
                this->link = link_str;
            }
        }
    }
    bool is_empty() const { return !enabled && link.isEmpty(); }
    void clear() {
        enabled = false;
        link = "";
    }
    QString to_str() const {
        if (is_empty()) return "";
        if (enabled) {
            return link;
        }
        else {
            return "#" + link;
        }
    }
    XLink get_enabled_link() const {
        XLink link = *this;
        link.enabled = true;
        return link;
    }
    XLink get_disabled_link() const {
        XLink link = *this;
        link.enabled = false;
        return link;
    }

};

//Parsing of link string
class XLinkParser {
public:
    //Clipboard:
    static const int Max_Link_Length = 100;   //constant for limiting link length from clibboard to avoit "giant" strings
    static const int Shorten_Link_Length = 30;   //constant for limiting link length when output at menu
    static QString get_link_from_clipboard(); //for text longer Max_Link_Length returns empty string
    static QString shorten_link(QString link); //for text longer Shorten_Link_Length changes end to "..."

    //Constructor
    XLinkParser() {}
    XLinkParser(QString module, QString var, int index = -1, int index2 = -1);
    XLinkParser(QString link);

    //String representation of link, such as `webcam1->image`
    QString to_str() const;

    bool has_index() const { return index >= 0; }

    bool is_empty = true;
    QString module;
    QString var;
    int index = -1; //if index >= 0 - interpret as a string separated by spaces 'a b c' and get its item 'index'
    int index2 = -1; //if index2 >= 0 - interpret as a string separated by "\n" and then spaces 'a b c' and get its line 'index' and item 'index2'


};


#endif // XLINK_H
