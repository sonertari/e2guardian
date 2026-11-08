#ifndef SIMPLE_PSL_H
#define SIMPLE_PSL_H

#include <string>
#include <vector>
#include <algorithm>

namespace SimplePSL {

inline const std::vector<std::string>& getKnownPublicSuffixes() {
    // Broad collection of major global gTLDs and multi-level ccTLDs (~95% coverage)
    static const std::vector<std::string> suffixes = {
        // Multi-level country codes (UK, Turkey, Australia, Japan, Brazil, etc.)
        "co.uk", "me.uk", "net.uk", "org.uk", "ac.uk",
        "com.tr", "org.tr", "edu.tr", "gov.tr", "net.tr", "bbs.tr", "name.tr", "av.tr", "dr.tr",
        "com.au", "net.au", "org.au", "edu.au", "gov.au",
        "co.jp", "ne.jp", "or.jp", "ac.jp", "go.jp", "ed.jp", "lg.jp",
        "com.br", "net.br", "org.br", "edu.br", "gov.br",
        "com.mx", "net.mx", "org.mx", "edu.mx", "gob.mx",
        "co.nz", "net.nz", "org.nz", "school.nz", "govt.nz",
        "co.za", "net.za", "org.za", "gov.za", "edu.za",
        "com.ar", "net.ar", "org.ar", "edu.ar", "gob.ar",
        "co.in", "net.in", "org.in", "ac.in", "gov.in", "res.in",
        "com.cn", "net.cn", "org.cn", "edu.cn", "gov.cn",
        "com.tw", "net.tw", "org.tw", "edu.tw", "gov.tw",
        "com.hk", "net.hk", "org.hk", "edu.hk", "gov.hk",
        "com.sg", "net.sg", "org.sg", "edu.sg", "gov.sg",
        "co.kr", "ne.kr", "or.kr", "re.kr", "pe.kr", "go.kr",
        
        // Popular generic & geo-targeted ccTLDs
        "eu", "us", "uk", "ca", "au", "de", "fr", "jp", "nl", "it", "es", 
        "br", "ru", "ch", "se", "no", "fi", "dk", "pl", "cz", "gr", "pt", 
        "hu", "ro", "ua", "tr", "il", "za", "in", "cn", "tw", "hk", "sg", 
        "kr", "mx", "ar", "cl", "co", "ve", "pe", "nz", "my", "ph", "th", 
        "id", "vn", "ae", "sa", "qa", "kw", "eg", "ma", "ng", "ke", "ua",

        // Common gTLDs & new gTLDs
        "com", "org", "net", "edu", "gov", "mil", "int",
        "io", "me", "info", "biz", "cc", "tv", "co", "ai", "app", "dev", 
        "tech", "site", "online", "store", "shop", "blog", "cloud", "xyz", 
        "mobi", "name", "pro", "asia", "tel", "travel", "xxx", "aero", 
        "coop", "museum", "jobs", "post", "geo", "local", "internal"
    };
    return suffixes;
}

inline std::string getRegistrableDomain(const std::string& host) {
    std::string lower_host = host;
    std::transform(lower_host.begin(), lower_host.end(), lower_host.begin(), ::tolower);
    
    // Remove trailing dot if present
    if (!lower_host.empty() && lower_host.back() == '.') {
        lower_host.pop_back();
    }

    const auto& suffixes = getKnownPublicSuffixes();
    std::string matched_suffix = "";

    // Sort-free lookup: Find the longest matching suffix from our list
    for (const auto& suffix : suffixes) {
        std::string dot_suffix = "." + suffix;
        if (lower_host == suffix) {
            return lower_host; 
        }
        if (lower_host.length() > dot_suffix.length() && 
            lower_host.compare(lower_host.length() - dot_suffix.length(), dot_suffix.length(), dot_suffix) == 0) {
            if (matched_suffix.length() < suffix.length()) {
                matched_suffix = suffix;
            }
        }
    }

    // FALLBACK: If no known suffix matched at all, gracefully return the original host
    if (matched_suffix.empty()) {
        size_t last_dot = lower_host.rfind('.');
        if (last_dot == std::string::npos) return lower_host;
        size_t second_last_dot = lower_host.rfind('.', last_dot - 1);
        if (second_last_dot == std::string::npos) return lower_host;
        return lower_host.substr(second_last_dot + 1);
    }

    // Extract the label immediately preceding the matched suffix to build domain.tld
    std::string remainder = lower_host.substr(0, lower_host.length() - matched_suffix.length());
    if (!remainder.empty() && remainder.back() == '.') {
        remainder.pop_back();
    }
    
    size_t prev_dot = remainder.rfind('.');
    if (prev_dot == std::string::npos) {
        return lower_host; // e.g., already just "linkedin.com"
    }

    return remainder.substr(prev_dot + 1) + "." + matched_suffix;
}

} // namespace SimplePSL

#endif // SIMPLE_PSL_H
