#ifndef SC_H
#define SC_H

#include<vector>
#include<limits>
#include<iostream>
#include <cstddef>
#include <stdexcept>//for exception handling
using namespace std;
using days_t = int;

template<typename T>
T safeInput(const string& prompt) {
    while (true) {
        cout << prompt;
        T value;
        if (cin >> value) {
            return value;
        }
        else {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input type. Please try again.\n";
        }
    }
}

//abstract base class
class FileItem {

protected:
    string name;
    size_t size_bytes;
    days_t days_since_access;
    int usage_count;
public:
    FileItem(string n, size_t s, days_t d, int u);
    virtual ~FileItem();

    const string& getName() const;
    size_t getSize() const;
    days_t getDaysSinceAccess() const;
    int getUsageCount() const;

    void touch(days_t d = 0);
    void incrementUsage();

    virtual double typeImportanceFactor() const = 0;
    virtual string typeName() const = 0;
    virtual void brief() const;

    bool operator<(const FileItem& other) const;
    bool operator>(const FileItem& other) const;
    size_t operator+(const FileItem& other) const;
 

};

// Derived classes
class Document : public FileItem {
    bool important_meta;
    string author;
    vector<string> tags;
public:
    Document(string n, size_t s, days_t d, int u, bool important = false);
    Document(const Document& other);
    Document& setAuthor(const string& a);
    Document& addTag(const string& t);
    Document& markImportant();

    double typeImportanceFactor() const override;
    string typeName() const override;
    void brief() const override;
};

class Photo : public FileItem {
protected:
    bool is_high_resolution;
    string location;
    string date_taken;
public:
    Photo(string n, size_t s, days_t d, int u, bool high_res = false);
    Photo(const Photo& other);
    Photo& setLocation(const string& loc);
    Photo& setDateTaken(const string& dt);
    Photo& markHighRes();

    double typeImportanceFactor() const override;
    string typeName() const override;
    void brief() const override;
};

class App : public FileItem {
    bool is_system_app;
    string version;
    string category;
public:
    App(string n, size_t s, days_t d, int u, bool system = false,
        string v = "1.0", string c = "Unknown");
    App(const App& other);
    App& setVersion(const string& v);
    App& setCategory(const string& c);
    App& markSystemApp();

    double typeImportanceFactor() const override;
    string typeName() const override;
    void brief() const override;
    friend class CloudBackupApp;
};
class CloudStored {
public:
    virtual bool hasBackup() const = 0;
    virtual string cloudProvider() const = 0;
    virtual ~CloudStored() {}
};


// CloudBackupApp
class CloudBackupApp : public App, public CloudStored {
    bool backed;
    string provider;

public:
    CloudBackupApp(string n, size_t s, days_t d, int u,
                   bool sys, string v, string c,
                   bool b, string p);
    CloudBackupApp(const CloudBackupApp& other);

    bool hasBackup() const ;
    string cloudProvider() const ;
    CloudBackupApp& setProvider(const string& p );

    void brief() const ;
};

// Sensitive Interface
class Sensitive {
public:
    virtual bool isSensitive() const = 0;
    virtual string sensitivityReason() const = 0;
    virtual ~Sensitive() {}
};

// PersonalPhoto
class PersonalPhoto : public Photo, public Sensitive {
public:
    string reason_sensitivity;
    PersonalPhoto(string n, size_t s, days_t d, int u, bool hr = true, string x = "");
    PersonalPhoto(const PersonalPhoto& other);
    bool isSensitive() const override;
    string sensitivityReason() const override;
    string typeName() const override;
    PersonalPhoto& setReason(const string& r);
    void brief() const override;
};

// Recommendation & Analysis
class Recommendation {
public:
    static const string KEEP;
    static const string ARCHIVE;
    static const string COMPRESS;
    static const string DELETE;
    static const string REVIEW;
};

class AnalysisResult {
    double score;
    string rec;
    string reason;
public:
    AnalysisResult(double s = 0.0, string r = "REVIEW", string re = "No reason");
    string getRec() const;
    string getReason() const;
    double getScore() const;
    friend class Analyzer;
};

class Analyzer {
    static const size_t LARGE_SIZE;
    static const size_t VERY_LARGE;
    static const days_t STALE_DAYS;
    static const days_t OLD_DAYS;

public:
    AnalysisResult analyze(const FileItem& f) const;
    template <typename Container>
    vector<std::pair<const FileItem*, AnalysisResult>>analyzeBatch(const Container& items) const;

private:
    double sizePenalty(size_t bytes) const;
};



//exception handling

class FileException : public runtime_error {
public:
    explicit FileException(const string& m) : runtime_error(m) {}
};
class InvalidInputError : public FileException {
public:
    explicit InvalidInputError(const string& m) : FileException(m) {}
};
class AnalysisError : public FileException {
public:
    explicit AnalysisError(const string& m) : FileException(m) {}
};
class MemoryError : public FileException {
public:
    explicit MemoryError(const string& m) : FileException(m) {}
};

template<typename Container>
void printBriefAll(Container& box);

#endif
