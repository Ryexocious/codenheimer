#ifndef SNIPPETBASECLASS_H
#define SNIPPETBASECLASS_H

#include <string>
// #include <QString>
#include <iostream>
#include <QTextDocument>
#include <QMainWindow>


/**
 * @brief 
 * 
 */
class snippetBaseClass
{

protected:
    std::string name;
    std::string filename;
    int lineNum;
    std::string lang;
    std::string SNIPPET;
    QMainWindow* masterWindow;

    // std::vector<snippetBaseClass*>vec;

public:
    snippetBaseClass();
    virtual ~snippetBaseClass();

    virtual void innit(
        std::string nam,
        std::string filenam,
        int linNum,
        std::string lng,
        std::vector<std::string> tgs,
        QMainWindow* MM
    )=0;

    virtual     std::vector<std::string> *getInfo(
        std::string &nam,
        std::string &lng
        )=0;

    virtual std::string getSnippet()=0;

    virtual std::string& EditSnippet()=0;

    virtual std::vector<std::string> getTags()=0;

    virtual void setEditor(QTextDocument*)=0;

    virtual std::string getLang()=0;
    
    virtual bool isCustom()=0;

    virtual void putColors(std::unordered_map<std::string,std::vector<std::string>>& colorsData)=0;

    virtual void putTags(std::vector<std::string>&)=0;

    virtual bool isLocked()=0;

// <<<<<<< ryexocious-making-search-page
    virtual void insert(const std::string& str, snippetBaseClass* targ) =0;
    
    virtual bool search(const std::string& str, std::vector<snippetBaseClass*>& ret)=0;

    virtual std::vector<std::pair<std::string, std::vector<snippetBaseClass*>>> searchWithPrefix(const std::string& prefix)=0;
// =======
    virtual bool saveSnippetToFile(std::string snippet)=0;

    virtual bool updateSnippetDetails(std::string nam,std::string filenam, std::vector<std::string> tgs, std::string lng, bool lock)=0;
// >>>>>>> main
    virtual bool updateSnippetFilename(std::string nam)=0;
    
    virtual std::string getOldFilename()=0;

    virtual bool deleteFromVault()=0;

    virtual bool remove(snippetBaseClass *obj)=0;
};

#endif // SNIPPETBASECLASS_H
