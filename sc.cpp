#include "sc.h"
#include<iostream>
#include<algorithm>
#include<string>
#include<cmath>
#include<vector>
using namespace std;

//fileItem

FileItem::FileItem(string n, size_t s, days_t d, int u)
    : name(move(n)), days_since_access(d), usage_count(u) {
    if (name.empty()) throw InvalidInputError("FileItem: name cannot be empty.");
    if (s == 0) throw InvalidInputError("FileItem: size must be > 0 MB.");
    if (d < 0) throw InvalidInputError("FileItem: days since access cannot be negative.");
    if (u < 0) throw InvalidInputError("FileItem: usage count cannot be negative.");
    const size_t MAX_MB = 200000;
    if (s > MAX_MB) throw InvalidInputError("FileItem: size too large.");
    size_bytes = s * 1024 * 1024;
    
}

FileItem::~FileItem() = default;
const string& FileItem::getName() const { return name; }
size_t FileItem::getSize() const { return size_bytes; }
days_t FileItem::getDaysSinceAccess() const { return days_since_access; }
int FileItem::getUsageCount() const { return usage_count; }

void FileItem::touch(days_t d) { days_since_access = d; }
void FileItem::incrementUsage() { ++usage_count; }

void FileItem::brief() const {
    cout << typeName() << ":" << name << " (" << size_bytes << "B, "
        << days_since_access << "d, u=" << usage_count << ")" << endl;
}

bool FileItem::operator<(const FileItem& other) const { return size_bytes < other.size_bytes; }
bool FileItem::operator>(const FileItem& other) const { return size_bytes > other.size_bytes; }
size_t FileItem::operator+(const FileItem& other) const {
    if (numeric_limits<size_t>::max() - size_bytes < other.size_bytes)
        throw overflow_error("FileItem::operator+: size overflow");
    return size_bytes + other.size_bytes;
}

//document

Document::Document(string n, size_t s, days_t d, int u, bool important)
    : FileItem(move(n), s, d, u), important_meta(important) {}
//copy constructor
Document::Document(const Document& other)
    : FileItem(other), important_meta(other.important_meta), author(other.author), tags(other.tags) {}

Document& Document::setAuthor(const string& a) { author = a; return *this; }
Document& Document::addTag(const string& t) { tags.push_back(t); return *this; }
Document& Document::markImportant() { important_meta = true; return *this; }

double Document::typeImportanceFactor() const { return important_meta ? 1.6 : 1.0; }
string Document::typeName() const { return "Document"; }

void Document::brief() const {
    cout << typeName() << ":" << name << " (" << size_bytes << "B, "
        << days_since_access << "d, u=" << usage_count
        << ", important=" << (important_meta ? "yes" : "no");
    if (!author.empty()) cout << ", author=" << author;
    if (!tags.empty()) {
        cout << ", tags=[";
        for (size_t i = 0; i < tags.size(); ++i) {
            if (i) cout << ", ";
            cout << tags[i];
        }
        cout << "]";
    }
    cout << ")" << endl;
}

// Photo

Photo::Photo(string n, size_t s, days_t d, int u, bool high_res)
    : FileItem(move(n), s, d, u), is_high_resolution(high_res),
    location("unknown"), date_taken("unknown") {}
Photo::Photo(const Photo& other)
    : FileItem(other), is_high_resolution(other.is_high_resolution),
    location(other.location), date_taken(other.date_taken) {}

Photo& Photo::setLocation(const string& loc) { location = loc; return *this; }
Photo& Photo::setDateTaken(const string& dt) { date_taken = dt; return *this; }
Photo& Photo::markHighRes() { is_high_resolution = true; return *this; }

double Photo::typeImportanceFactor() const { return is_high_resolution ? 1.3 : 1.05; }
string Photo::typeName() const { return "Photo"; }

void Photo::brief() const {
    cout << typeName() << ":" << name << " (" << size_bytes << "B, "
        << days_since_access << "d, u=" << usage_count
        << ", loc=" << location << ", taken=" << date_taken << ")" << endl;
}

// App

App::App(string n, size_t s, days_t d, int u, bool system, string v, string c)
    : FileItem(move(n), s, d, u), is_system_app(system), version(move(v)), category(move(c)) {}
App::App(const App& other)
    : FileItem(other), is_system_app(other.is_system_app), version(other.version), category(other.category) {}

App& App::setVersion(const string& v) { version = v; return *this; }
App& App::setCategory(const string& c) { category = c; return *this; }
App& App::markSystemApp() { is_system_app = true; return *this; }

double App::typeImportanceFactor() const { return is_system_app ? 2.0 : 0.8; }
string App::typeName() const { return "App"; }

void App::brief() const {
    cout << typeName() << ":" << name << " (" << size_bytes << "B, "
        << days_since_access << "d, u=" << usage_count
        << ", ver=" << version << ", cat=" << category << ")" << endl;
}


// PersonalPhoto

PersonalPhoto::PersonalPhoto(string n, size_t s, days_t d, int u, bool hr, string x)
    : Photo(move(n), s, d, u, hr), reason_sensitivity(move(x)) {}
PersonalPhoto::PersonalPhoto(const PersonalPhoto& other)
    : Photo(other), reason_sensitivity(other.reason_sensitivity) {}
bool PersonalPhoto::isSensitive() const { return true; }
string PersonalPhoto::sensitivityReason() const { return reason_sensitivity; }
string PersonalPhoto::typeName() const { return "PersonalPhoto"; }
PersonalPhoto& PersonalPhoto::setReason(const std::string& r) {
    reason_sensitivity = r;
    return *this;
}

void PersonalPhoto::brief() const {
    cout << typeName() << ":" << name << " (" << size_bytes << "B, "
        << days_since_access << "d, u=" << usage_count
        << ", loc=" << location << ", taken=" << date_taken << ")";
    if (!reason_sensitivity.empty())
        cout << " [sensitivity: " << reason_sensitivity << "]";
    cout << endl;
}

// CloudBackupApp

CloudBackupApp::CloudBackupApp(string n, size_t s, days_t d, int u, bool sys, string v, string c, bool b, string p)
    : App(n, s, d, u, sys, v, c), backed(b), provider(p) {}
CloudBackupApp::CloudBackupApp(const CloudBackupApp& other):App(other),backed(other.backed),provider(other.provider){}


bool CloudBackupApp::hasBackup() const { return backed; }
string CloudBackupApp::cloudProvider() const { return provider; }
 CloudBackupApp& CloudBackupApp:: setProvider(const std::string& p ){  provider = p; return *this;}

void CloudBackupApp::brief() const {
    cout << typeName() << ":" << name << " (" 
         << size_bytes << "B, "
         << days_since_access << "d, u=" << usage_count
         << ", ver=" << version << ", cat=" << category
         << ", backup=" << (backed ? "yes" : "no")
         << ", provider=" << provider
         << ")" << endl;
}

//Recommendation constants

const string Recommendation::KEEP = "KEEP";
const string Recommendation::ARCHIVE = "ARCHIVE";
const string Recommendation::COMPRESS = "COMPRESS";
const string Recommendation::DELETE = "DELETE";
const string Recommendation::REVIEW = "REVIEW";

// AnalysisResult
AnalysisResult::AnalysisResult(double s, string r, string re)
    : score(s), rec(move(r)), reason(move(re)) {}

string AnalysisResult::getRec() const { return rec; }
string AnalysisResult::getReason() const { return reason; }
double AnalysisResult::getScore() const { return score; }


// Analyzer
const size_t Analyzer::LARGE_SIZE = 50ULL * 1024ULL * 1024ULL;   // 50 MB
const size_t Analyzer::VERY_LARGE = 500ULL * 1024ULL * 1024ULL; // 500 MB
const days_t Analyzer::STALE_DAYS = 365;                   // 1 year
const days_t Analyzer::OLD_DAYS = 3 * 365;                 // 3 years

AnalysisResult Analyzer::analyze(const FileItem& f) const {

    if (f.getSize() == 0)
        throw AnalysisError("Analyzer::analyze: file size is 0.");
    if (f.getDaysSinceAccess() < 0)
        throw AnalysisError("Analyzer::analyze: negative days since access.");
    if (f.getUsageCount() < 0)
        throw AnalysisError("Analyzer::analyze: negative usage count.");

    double usage_component = min(1.0, f.getUsageCount() / 30.0);
    double recency_component = 1.0 / (1.0 + f.getDaysSinceAccess() / 30.0);
    double size_penalty = sizePenalty(f.getSize());
    double type_factor = f.typeImportanceFactor();

    double score = (0.6 * usage_component + 0.4 * recency_component) * type_factor - size_penalty;
    if (score < -2.0) score = -2.0;
    if (score > 3.0) score = 3.0;

    string rec;
    string reason;
       // Cloud backup rule (highest priority)

    const CloudStored* cb = dynamic_cast<const CloudStored*>(&f);
    if (cb && cb->hasBackup() && f.getSize() > VERY_LARGE) {
        rec = Recommendation::COMPRESS;
        reason = "Large app with cloud backup - safe to compress.";
    }
    if (f.getSize() >= VERY_LARGE && usage_component < 0.2 && f.getDaysSinceAccess() > STALE_DAYS) {
        rec = Recommendation::ARCHIVE;
        reason = "Very large, rarely used - consider archiving to external storage.";
    }
    else if (score < -0.5 && f.getDaysSinceAccess() > OLD_DAYS && usage_component < 0.1) {
        rec = Recommendation::DELETE;
        reason = "Old, unused, and low importance - candidate for deletion.";
    }
    else if (f.getSize() >= LARGE_SIZE && score < 0.5) {
        rec = Recommendation::COMPRESS;
        reason = "Large file and low-to-medium importance - compression recommended.";
    }
    else if (score > 1.0) {
        rec = Recommendation::KEEP;
        reason = "High relevance / recent usage - keep.";
    }
    else {
        rec = Recommendation::REVIEW;
        reason = "Ambiguous - manual review recommended.";
    }

    return AnalysisResult(score, rec, reason);
}

// Batch analyzer
// Batch analyzer
template<typename Container>
vector<pair<const FileItem*, AnalysisResult>>
Analyzer::analyzeBatch(const Container& items) const {

    vector<pair<const FileItem*, AnalysisResult>> out;
    out.reserve(items.size());

    for (FileItem* p : items) {
        if (p == nullptr)
            continue;

        try {
            out.emplace_back(p, analyze(*p));
        }
        catch (const std::exception& e) {
            out.emplace_back(
                p,
                AnalysisResult(
                    0.0,
                    Recommendation::REVIEW,
                    string("Analysis failed: ") + e.what()
                )
            );
        }
    }

    sort(out.begin(), out.end(),
        [](const auto& a, const auto& b) {
            return a.second.getScore() > b.second.getScore();
        });

    return out;
}

// Improved size penalty calculation
double Analyzer::sizePenalty(size_t bytes) const {
    double mb = bytes / (1024.0 * 1024.0);
    if (mb < 5.0) return 0.0;              // small files   no penalty
    if (mb < 50.0) return (mb - 5.0) / 100.0; // mild penalty starts after 5MB
    if (mb < 500.0) return 0.5 + (mb - 50.0) / 500.0; // stronger for big files
    return 1.5 + log10(mb / 500.0);        // exponential penalty for huge files
}

template <typename Container>
void printBriefAll(Container& box) {
    for (FileItem* f : box)
        f->brief();
}




// add()

template vector<pair<const FileItem*, AnalysisResult>>
Analyzer::analyzeBatch<vector<FileItem*>>(const vector<FileItem*>&) const;
template void printBriefAll<vector<FileItem*>>(vector<FileItem*>&);
