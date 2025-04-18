#include "searchsyetem.h"
#include "qlogging.h"
#include "snippetbaseclass.h"

int searchSystem::getIndex(char ch) {
    if (ch == ' ') return 54;
    if (ch == '-') return 53;
    if(ch =='_') return 55;
    if (isupper(ch)) return ch - 'A';
    if (islower(ch)) return ch - 'a' + 26;
    if(std::isdigit(ch)){
        return ch-'0'+56;
    }
    return -1; // Invalid character
}

void searchSystem::clear(Node *curr) {
    if (!curr) return;
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        clear(curr->next[i]);
    }
    delete curr;
}

searchSystem::Node* searchSystem::collectAll(Node *curr, std::string prefix, std::vector<std::pair<std::string, std::vector<snippetBaseClass*>>> &results) {
    if (!curr) return nullptr;
    if (curr->endpoint) {
        results.emplace_back(prefix, curr->target);
    }
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        if (curr->next[i]) {
            char ch;
            if (i < 26) ch = 'A' + i;
            else if (i < 52) ch = 'a' + (i - 26);
            else if (i == 53) ch = '-';
            else if (i == 54) ch = ' ';
            collectAll(curr->next[i], prefix + ch, results);
        }
    }
    return curr;
}


void searchSystem::collectAll(int& skipCount, int& remainingCount, Node* curr,
                              std::string prefix, std::vector<std::pair<std::string, std::vector<snippetBaseClass*>>>& results) {

    if (!curr || remainingCount <= 0) return;

    if (curr->endpoint) {
        if (skipCount > 0) {
            skipCount--; // Ignore this snippet
        } else {
            results.emplace_back(prefix, curr->target);
            remainingCount--;
        }
    }

    for (int i = 0; i < ALPHABET_SIZE && remainingCount > 0; i++) {
        if (curr->next[i]) {
            char ch;
            if (i < 26) ch = 'A' + i;
            else if (i < 52) ch = 'a' + (i - 26);
            else if (i == 53) ch = '-';
            else if (i == 54) ch = ' ';

            collectAll(skipCount, remainingCount, curr->next[i], prefix + ch, results);
        }
    }
}


searchSystem::searchSystem() {
    root = new Node;
}

searchSystem::~searchSystem() {
    clear(root);
}

void searchSystem::innit(std::string nam, std::string filenam, int linNum, std::string lng, std::vector<std::string> tgs, QMainWindow *MM)
{
}

std::vector<std::string> *searchSystem::getInfo(std::string &nam, std::string &lng)
{
    return nullptr;
}

std::string searchSystem::getSnippet() {
    return "";
}

std::vector<std::string> searchSystem::getTags() {
    return {};
}

void searchSystem::setEditor(QTextDocument *) {}

std::string searchSystem::getLang() {
    return "";
}

bool searchSystem::isCustom() {
    return true;
}

void searchSystem::putColors(std::unordered_map<std::string, std::vector<std::string>> &colorsData) {}

void searchSystem::putTags(std::vector<std::string> &) {}

bool searchSystem::isLocked() {
    return true;
}

void searchSystem::tellTotalCount(int t)
{
    totalCount=t;
}

void searchSystem::insert(const std::string &str, snippetBaseClass *targ) {
    Node* curr = root;
    for (char ch : str) {
        int idx = getIndex(ch);
        if (idx == -1) continue; // Skip invalid characters
        if (!curr->next[idx]) {
            curr->next[idx] = new Node;
        }
        curr = curr->next[idx];
    }
    curr->endpoint = true;
    curr->target.push_back(targ);
}

bool searchSystem::search(const std::string &str, std::vector<snippetBaseClass*>& ret) {
    Node* curr = root;
    for (char ch : str) {
        int idx = getIndex(ch);
        if (idx == -1 || !curr->next[idx]) {
            return false;
        }
        curr = curr->next[idx];
    }
    ret = curr->target;
    return curr->endpoint;
}

std::vector<std::pair<std::string, std::vector<snippetBaseClass*>>> searchSystem::searchWithPrefix(const std::string &prefix) {
    Node* curr = root;
    std::vector<std::pair<std::string, std::vector<snippetBaseClass*>>> results;

    for (char ch : prefix) {
        int idx = getIndex(ch);
        if (idx == -1 || !curr->next[idx]) {
            return results; // Prefix not found
        }
        curr = curr->next[idx];
    }

    collectAll(curr, prefix, results);
    return results;
}

bool searchSystem::remove(snippetBaseClass* obj){
    std::string name;
    std::string str;
    obj->getInfo(name,str);

    Node* curr = root;
    for (char ch : name) {
        int idx = getIndex(ch);
        if (idx == -1 || !curr->next[idx]) {
            return false;
        }
        curr = curr->next[idx];
    }
    curr->target.erase( std::find( curr->target.begin() , curr->target.end() , obj ) );
    if(curr->target.size() <=0 ) curr->endpoint=false;
    return true;
}

bool searchSystem::rename(std::string newName , snippetBaseClass* obj){
    remove(obj);
    // std::string nam;
    // std::string str;
    // obj->getInfo(nam,str);
    insert(newName,obj);
}


void searchSystem::display(Node *curr, std::string str) {
    if (!curr) {
        std::cout << "\nTree has:\n";
        curr = root;
    }
    if (curr->endpoint) {
        std::cout << str << std::endl;
    }
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        if (curr->next[i]) {
            char ch;
            if (i < 26) ch = 'A' + i;
            else if (i < 52) ch = 'a' + (i - 26);
            else if (i == 53) ch = '-';
            else if (i == 54) ch = ' ';
            display(curr->next[i], str + ch);
        }
    }
}

std::vector<std::pair<std::string, std::vector<snippetBaseClass*>>> searchSystem::pagedSearch(int n, bool continueSearch) {
    std::vector<std::pair<std::string, std::vector<snippetBaseClass*>>> results;

    if (!continueSearch) { // Restart search, reset attempt count
        qDebug() << "Restarting search from root...";
        searchAttempt = 0;
    }
    //  else {
    //     searchAttempt++;
    // }

    //int searchLimit = searchAttempt * n;   // Total nodes we should have seen so far
    //int skipCount = (searchAttempt - 1) * n; // How many nodes to skip

    if(searchAttempt > (totalCount+n)) searchAttempt=0;
    int skipCount = searchAttempt;
    int remainingCount = n; // We only collect `n` snippets for the current page

    qDebug() << "Initiating paged search: skipping " << skipCount
             << " and collecting " << remainingCount << "...";

    collectAll(skipCount, remainingCount, root, "", results);
    searchAttempt += n;
    return results;
}

bool searchSystem::saveSnippetToFile(std::string snippet)
{
    return false;
}

bool searchSystem::updateSnippetDetails(std::string nam, std::string filenam, std::vector<std::string> tgs, std::string lng, bool lock)
{
    return false;
}

std::string &searchSystem::EditSnippet()
{
    // TODO: insert return statement here
}

bool searchSystem::updateSnippetFilename(std::string nam)
{
    return false;
}

std::string searchSystem::getOldFilename()
{
    return "";
}

bool searchSystem::deleteFromVault()
{
    return false;
}
