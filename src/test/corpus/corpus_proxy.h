#include "corpus_database.h"
#include <string>
#include <list>

enum CorpusLanguage {
    CORPUS_CHINESE,
    CORPUS_ENGLISH,
    CORPUS_JAPANESE,
    CORPUS_RUSSIAN,
    CORPUS_KOREAN,
    CORPUS_MAX
};

enum VisitPermission {
    PERMISSION_NONE = -1,
    PERMISSION_BORWSE,
    PERMISSION_DWONLOAD,
    PERMISSION_UPLOAD,
    PERMISSION_MAX
};

struct context_info {
    std::string  ftp_path_;
    std::wstring file_name_;
    std::wstring full_context_;
    int			 character_count_;
};

typedef std::list<context_info*>	list_context_info;

class CorpusProxy {
  public:

    CorpusProxy() {
        select_character_count_[CORPUS_CHINESE] = "SELECT SUM(character_count) FROM t_chinese;";
        select_character_count_[CORPUS_ENGLISH] = "SELECT SUM(character_count) FROM t_english;";
        select_character_count_[CORPUS_JAPANESE] = "SELECT SUM(character_count) FROM t_japanese;";
        select_character_count_[CORPUS_RUSSIAN] = "SELECT SUM(character_count) FROM t_russian;";
        select_character_count_[CORPUS_KOREAN] = "SELECT SUM(character_count) FROM t_korean;";

        select_record_[CORPUS_CHINESE] = "SELECT COUNT(*) context FROM t_chinese;";
        select_record_[CORPUS_ENGLISH] = "SELECT COUNT(*) context FROM t_english";
        select_record_[CORPUS_JAPANESE] = "SELECT COUNT(*) context FROM t_japanese;";
        select_record_[CORPUS_RUSSIAN] = "SELECT COUNT(*) context FROM t_russian;";
        select_record_[CORPUS_KOREAN] = "SELECT COUNT(*) context FROM t_korean;";

    }

    bool Connect(const std::string& host,const std::string& user_name,const std::string& password) {
        //	"localhost;3306;root;sbivfh;language_database";
        std::string database_info = host + ";3306;" + user_name + ";" + password + ";" + "corpus";
        return corpus_database_.Open(database_info,0,1);
    }

    void AddUser(const std::string& user_name,const std::string& password,VisitPermission permission) {
        //	make sure the user is unique
        if (PERMISSION_NONE != Login(user_name,password)) {
            return;
        }
        easy::EasyPreparedStatement* pre_stmt = NULL;
        pre_stmt = corpus_database_.GetPreparedStatement(CORPUS_ADD_USER);
        pre_stmt->setString(0,user_name);
        pre_stmt->setString(1,password);
        pre_stmt->setInt32(2,static_cast<easy_int32>(permission));
        corpus_database_.DirectExecute(pre_stmt);
    }

    VisitPermission Login(const std::string& user_name,const std::string& password) const {
        CorpusProxy* myself = const_cast<CorpusProxy*>(this);
        VisitPermission permission = PERMISSION_NONE;
        easy::EasyPreparedStatement* pre_stmt = NULL;
        pre_stmt = myself->corpus_database_.GetPreparedStatement(CORPUS_USER_CHECK);
        pre_stmt->setString(0,user_name);
        pre_stmt->setString(1,password);
        easy::PreparedQueryResult pre_result_result = myself->corpus_database_.Query(pre_stmt);
        if (pre_result_result) {
            do {
                easy::EasyField* fields = pre_result_result->Fetch();
                permission = static_cast<VisitPermission>(fields[0].GetInt32());
            } while (pre_result_result->NextRow());
        }
        myself->cur_user_ = user_name;
        return permission;
    }

    //	return a record, usually the fist find value
    //	part_context can be a keyword,word,sentence,and the character must be wide-char
    void Search(CorpusLanguage language,std::wstring part_context,std::wstring& file_name,std::string& ftp_path,std::wstring& full_context,int& character_count) {
        easy::EasyPreparedStatement* pre_stmt = NULL;
        pre_stmt = corpus_database_.GetPreparedStatement(CORPUS_SEL_CHINESE + language);
        _string_extra(part_context);
        pre_stmt->setString(0,_UnicodeToUTF8(part_context));
        corpus_database_.DirectExecute(pre_stmt);
        easy::PreparedQueryResult pre_result_chinese = corpus_database_.Query(pre_stmt);
        if (pre_result_chinese) {
            do {
                easy::EasyField* fields = pre_result_chinese->Fetch();
                file_name = _UTF8ToUnicode(fields[0].GetString());
                ftp_path = fields[1].GetString();
                full_context = _UTF8ToUnicode(fields[2].GetString());
                character_count = fields[3].GetInt32();
                break;
            } while (pre_result_chinese->NextRow());
        }
    }

    //	return multi record,include all
    void Search(CorpusLanguage language,std::wstring part_context,list_context_info& __list_context_info) {
        easy::EasyPreparedStatement* pre_stmt = NULL;
        pre_stmt = corpus_database_.GetPreparedStatement(CORPUS_SEL_CHINESE + language);
        _string_extra(part_context);
        pre_stmt->setString(0,_UnicodeToUTF8(part_context));
        corpus_database_.DirectExecute(pre_stmt);
        easy::PreparedQueryResult pre_result_chinese = corpus_database_.Query(pre_stmt);
        if (pre_result_chinese) {
            do {
                easy::EasyField* fields = pre_result_chinese->Fetch();
                context_info* __context_info = new context_info();
                if (__context_info) {
                    __context_info->file_name_ = _UTF8ToUnicode(fields[0].GetString());
                    __context_info->ftp_path_ = fields[1].GetString();
                    __context_info->full_context_ = _UTF8ToUnicode(fields[2].GetString());
                    __context_info->character_count_ = fields[3].GetInt32();
                    __list_context_info.push_back(__context_info);
                }
            } while (pre_result_chinese->NextRow());
        }
    }

    //	return multi record,only __record_num records from the __record_index_start th.
    void Search(CorpusLanguage language,std::wstring part_context,list_context_info& __list_context_info,unsigned int __record_index_start,unsigned int __record_num = 50) {
        easy::EasyPreparedStatement* pre_stmt = NULL;
        pre_stmt = corpus_database_.GetPreparedStatement(CORPUS_SEL_LIMIT_CHINESE + language);
        _string_extra(part_context);
        pre_stmt->setString(0,_UnicodeToUTF8(part_context));
        pre_stmt->setUInt32(1,__record_index_start);
        pre_stmt->setUInt32(2,__record_num);

        corpus_database_.DirectExecute(pre_stmt);
        easy::PreparedQueryResult pre_result_chinese = corpus_database_.Query(pre_stmt);
        if (pre_result_chinese) {
            do {
                easy::EasyField* fields = pre_result_chinese->Fetch();
                context_info* __context_info = new context_info();
                if (__context_info) {
                    __context_info->file_name_ = _UTF8ToUnicode(fields[0].GetString());
                    __context_info->ftp_path_ = fields[1].GetString();
                    __context_info->full_context_ = _UTF8ToUnicode(fields[2].GetString());
                    __context_info->character_count_ = fields[3].GetInt32();
                    __list_context_info.push_back(__context_info);
                }
            } while (pre_result_chinese->NextRow());
        }
    }

    //	count the record in all in one language by some special character
    unsigned int Count(CorpusLanguage language,std::wstring part_context) {
        easy::EasyPreparedStatement* pre_stmt = NULL;
        pre_stmt = corpus_database_.GetPreparedStatement(CORPUS_SEL_COUNT_CHINESE + language);
        _string_extra(part_context);
        pre_stmt->setString(0,_UnicodeToUTF8(part_context));

        corpus_database_.DirectExecute(pre_stmt);
        easy::PreparedQueryResult pre_result_chinese = corpus_database_.Query(pre_stmt);
        if (pre_result_chinese) {
            easy::EasyField* fields = pre_result_chinese->Fetch();
            if (fields) {
                return fields[0].GetUInt32();
            }
        }
        return 0;
    }

    //	count the record in all in one language
    unsigned int Count(CorpusLanguage language) {
        unsigned int __record_count = 0;
        easy::EasyQueryResult result_set = corpus_database_.Query(select_record_[language].c_str());
        if (result_set) {
            easy::EasyField *fields = result_set->Fetch();
            if (fields) {
                __record_count = fields[0].GetUInt32();
            }
        }
        return __record_count;
    }

    //	count the character in all in one language
    unsigned int Character_Count(CorpusLanguage language) {
        unsigned int __character_count = 0;
#if 0
        easy::EasyPreparedStatement* pre_stmt = NULL;
        pre_stmt = corpus_database_.GetPreparedStatement(CORPUS_SEL_CHARACTER_COUNT_CHINESE + language);;
        corpus_database_.DirectExecute(pre_stmt);
        easy::PreparedQueryResult pre_result_chinese = corpus_database_.Query(pre_stmt);
        if (pre_result_chinese) {
            easy::EasyField* fields = pre_result_chinese->Fetch();
            if (fields) {
                __character_count = fields[0].GetUInt32();
            }
        }
#else
        easy::EasyQueryResult result_set = corpus_database_.Query(/*"select sum(character_count) from t_chinese;"*/select_character_count_[language].c_str());
        if (result_set) {
            easy::EasyField *fields = result_set->Fetch();
            if (fields) {
                __character_count = fields[0].GetUInt32();
            }
        }
#endif
        return __character_count;
    }

    bool Upload(CorpusLanguage language,const std::wstring& file_name,const std::wstring& context,const std::string& ftp_path,int character_count) {
        bool res = false;
        easy::EasyPreparedStatement* pre_stmt = NULL;
        pre_stmt = corpus_database_.GetPreparedStatement(CORPUS_ADD_CHINESE + language);
        pre_stmt->setString(0,_UnicodeToUTF8(file_name));
        pre_stmt->setString(1,_UnicodeToUTF8(context));
        pre_stmt->setString(2,ftp_path);
        pre_stmt->setInt32(3,character_count);
        res = corpus_database_.DirectExecute(pre_stmt);
        //	add upload log information
        pre_stmt = NULL;
        pre_stmt = corpus_database_.GetPreparedStatement(CORPUS_ADD_LOG);
        pre_stmt->setString(0,cur_user_);
        corpus_database_.DirectExecute(pre_stmt);
        return res;
    }

    bool Delete(CorpusLanguage language,const std::wstring& file_name) {
        bool res = false;
        easy::EasyPreparedStatement* pre_stmt = NULL;
        pre_stmt = corpus_database_.GetPreparedStatement(CORPUS_DEL_CHINESE + language);
        pre_stmt->setString(0,_UnicodeToUTF8(file_name));;
        res = corpus_database_.DirectExecute(pre_stmt);
        return res;
    }

    std::string UnicodeToAnsi( const std::wstring& val ) {
        return _UnicodeToAnsi(val);
    }

  private:
    std::wstring _UTF8ToUnicode( const std::string& val ) {
        size_t len = 0;
        len = val.length();
        int unicodeLen = ::MultiByteToWideChar( CP_UTF8, 0, val.c_str(), -1, NULL, 0 );
        wchar_t * unicode;
        unicode = new wchar_t[unicodeLen+1];
        memset(unicode,0,(unicodeLen+1)*sizeof(wchar_t));
        ::MultiByteToWideChar( CP_UTF8, 0, val.c_str(), -1, (LPWSTR)unicode, unicodeLen );
        std::wstring rt;
        rt = ( wchar_t* )unicode;
        delete unicode;

        return rt;
    }

    std::wstring _AnsiToUnicode( const std::string& val ) {
        size_t len = 0;
        len = val.length();
        int unicodeLen = ::MultiByteToWideChar( CP_ACP, 0, val.c_str(), -1, NULL, 0 );
        wchar_t * unicode;
        unicode = new wchar_t[unicodeLen+1];
        memset(unicode,0,(unicodeLen+1)*sizeof(wchar_t));
        ::MultiByteToWideChar( CP_ACP, 0, val.c_str(), -1, (wchar_t*)unicode, unicodeLen );
        std::wstring rt;
        rt = ( wchar_t* )unicode;
        delete unicode;
        return rt;
    }

    std::string _UnicodeToUTF8( const std::wstring& val ) {
        char* element_text;
        int    text_len;
        // wide char to multi char
        text_len = ::WideCharToMultiByte( CP_UTF8, 0, val.c_str(), -1, NULL, 0, NULL, NULL );
        element_text = new char[text_len + 1];
        memset( ( void* )element_text, 0, sizeof( char ) * ( text_len + 1 ) );
        ::WideCharToMultiByte( CP_UTF8, 0, val.c_str(), -1, element_text, text_len, NULL, NULL );
        std::string strText;
        strText = element_text;
        delete[] element_text;
        return strText;
    }

    std::string _UnicodeToAnsi( const std::wstring& val ) {
        char* element_text;
        int    text_len;
        // wide char to multi char
        text_len = WideCharToMultiByte( CP_ACP, 0, val.c_str(), -1, NULL, 0, NULL, NULL );
        element_text = new char[text_len + 1];
        memset( ( void* )element_text, 0, sizeof( char ) * ( text_len + 1 ) );
        ::WideCharToMultiByte( CP_ACP, 0, val.c_str(), -1, element_text, text_len, NULL, NULL );
        std::string strText = element_text;
        delete[] element_text;
        return strText;
    }

    std::string _AnsiToUTF8(const std::string& val) {
        return _UnicodeToUTF8(_AnsiToUnicode(val));
    }

    void _string_extra(std::wstring& __source) {
#if 0
        std::wstring __temp = L"%";
        __temp += __source;
        __temp += L"%";
        __source = __temp;
#else
        __source.insert(__source.begin(),L'%');
        __source.push_back(L'%');
#endif

    }

    CorpusProxy ( const CorpusProxy& );
    CorpusProxy& operator= (const CorpusProxy&);

  private:
    CorpusDatabaseWorkerPool								corpus_database_;
    std::string												cur_user_;
    std::string												select_record_[CORPUS_MAX];
    std::string												select_character_count_[CORPUS_MAX];

};