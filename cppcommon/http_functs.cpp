#include "http_functs.h"

namespace CPPCOMMON
{
    bool parseUrl(const string& url, HashMap<string, string>& mp)
    {
        if(0 == url.length())
        {
            return false;
        }

        uint pos = url.find('?');
        uint kleft = 0, kright = 0;
        uint vleft = 0, vright = 0;
        for(uint i = pos + 1; i < url.size();)
        {
            cout<<i<<endl;
            kleft = i;
            while(i < url.size() && url[i] != '=')
            {
                i++;
            }
            if(i >= url.size())
            {
                break;
            }
            kright = i;
            i++;
            vleft = i;
            while(i < url.size() && url[i] != '&')
            {
                i++;
            }
            vright = i;
            i++;

            mp[url.substr(kleft, kright - kleft)] = url.substr(vleft, vright - vleft);

        }

        return true;
    }

    const char* const HttpReqInfo::KEY_METHOD = "METHOD";
    const char* const HttpReqInfo::KEY_PATH = "PATH";
    const char* const HttpReqInfo::KEY_PROTOCOL = "PROTOCOL";

    bool HttpReqInfo::load(const string& headerStr)
    {
        size_t len = headerStr.size();
        size_t lpos = 0, rpos = 0;
        vector<string> buf;
        rpos = headerStr.find('\n', lpos);
        if(string::npos == rpos || !splitStr(headerStr.substr(lpos, rpos - lpos), buf, " ") || 3 != buf.size())
        {
            LogError("parse header first line failed.");
            return false;
        }
        _headerMap[KEY_METHOD] = trim(buf[0]); 
        _headerMap[KEY_PATH] = trim(buf[1]); 
        _headerMap[KEY_PROTOCOL] = trim(buf[2]); 
        return true;
    }

    bool HttpReqInfo::_parse(const string& headerStr, size_t lpos, size_t rpos, const char * const key)
    {
        if(string::npos == rpos || rpos <= lpos)
        {
            return false;
        }
        string s(headerStr.substr(lpos, rpos - lpos));
        _headerMap[KEY_METHOD] = trim(s);
        return true;
    }

    bool HttpReqInfo::_methodMap(const HashMap<string, string>& mp, const string& key, string& res)
    {
        HashMap<string, string>::const_iterator it = mp.find(key);
        if(it == mp.end())
        {
            return false;
        }
        res = it->second;
        return true;
    }

#ifdef DEBUG
    string HttpReqInfo::toString()
    {
        string res("{");
        res += HashMapToString(_headerMap);
        res += ",";
        res += HashMapToString(_methodGetMap);
        res += ",";
        res += HashMapToString(_methodPostMap);
        res += "}";
        return res;
    }
#endif

}


#ifdef UT

using namespace CPPCOMMON;
int main()
{
    string url("http://127.0.0.1/?k1=v1&k2=v2 hh");
    HashMap<string, string> mp;
    parseUrl(url, mp);
    cout<<HashMapToString(mp)<<endl;
    const char * header = "GET /hehek1=v1&%20k2=v2 HTTP/1.1\nHost: 10.109.245.13:11256\nConnection: keep-alive\nAccept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\nUser-Agent: Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/29.0.1547.66 Safari/537.36\nAccept-Encoding: gzip,deflate,sdch\nAccept-Language: zh-CN,zh;q=0.8\n";
    HttpReqInfo reqinfo;
    reqinfo.load(header);
    cout<<reqinfo.toString()<<endl;
    return 0;
}

#endif