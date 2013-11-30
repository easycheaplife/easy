#include <string>
#include "easy_cppunit_proxy.h"
#include "easy_data_base_worker_pool.h"
#include "easy_log.h"

#pragma warning ( disable : 4566)
#pragma warning ( disable : 4996)

const static std::wstring chinese = L"曲曲折折的荷塘上面，弥望的是田田的叶子。叶子出水很高，像亭亭的舞女的裙。层层的叶子中间，\
					  零星地点缀着些白花，有袅娜地开着的，有羞涩地打着朵儿的；正如一粒粒的明珠，又如碧天里的星星，又如刚出浴的美人。\
					  微风过处，送来缕缕清香，仿佛远处高楼上渺茫的歌声似的。这时候叶子与花也有一丝的颤动，像闪电般，霎时传过荷塘的那边去了。\
					  叶子本是肩并肩密密地挨着，这便宛然有了一道凝碧的波痕。叶子底下是脉脉的流水，遮住了，不能见一些颜色；而叶子却更见风致了。";

const static std::wstring english = L"Above the winding lotus pond, with quartered lines leaves. Leaves are high above the water, \
	like the skirts of. Layers of leaves, dotted with some white, a demure bloom, some shyly in;\
	As YiLiLi pearl and like stars in the scattering pearls, and beauties just out of the bath. \
	A breeze stirs, sending over breaths fragrance, like the drifting from a distant building singing. \
	Leaves and flowers have at this time a tremor, like lightning, streaking across the lotus pond of the other side. \
	Leaves are shoulder to, which are caught shimmering in a patch of ripples. Underneath, the exquisite water is hidden from view, \
	can't see some color; But more see leaves and charming.";

const static std::wstring janpanese = L"荷塘上折れ曲がったのは田田真っ青の葉。葉と水が高く、像は美女の踊り子の裳裾だった。\
					  幾重にもの葉の真ん中で、に、点々として、何白い花が饰りながら、ヤナギに咲いていたものであり、\
					  はにかむように懐かしげに朵儿;おばさんの名手、またオン・オフするようになった」と碧天里の星がないにもかかわらず、まるで出浴の美人。\
					  そよ风を送って来た地点を見れば、その缕缕移り香遠く高楼にはかないの歌声をしているようだ。\
					  ヨシヨウコが、花もが少しでもたるん、雷のように、一瞬の噂があっ荷塘の向こうに行ってきました。\
					  葉はもともとは肩を憂えて密密地があったという事実は宛然凝碧の波を動かそうとしておきます。\
					  葉の流れをその下はとして、見えない一部の色;葉は遥かに会っ风致しました。";

const static std::wstring russian = L"Изгибов и поворотов над прудом, Ми Ван Тада листьев. \
	Листья воды высок, таких как юбка Kingston танцора.Средние слои листьев,\
	отделаны несколько белых цветов рассеяны местах, чтобы открыть гибкая,\
	с именем застенчивый БАДов, как капсулы жемчужина, и если Bitian в звездах, \
	а если просто купания красотой. Бриз через офис, послал нити аромат, \
	как на далекой высотных тонкий, как песня. \
	В это время листья и цветки имеют следов вибрации, как молния, мгновенно переходить на \
	другую сторону пруда.Толстые листья бок о бок к следующему, \
	который бы Ningbi сердцем рябь. Нежность уходит под воду, накрыть крышкой,\
	и не можете увидеть некоторые из цветов, и вызвать листьев больше ветра.";

const static std::wstring korean = L"한 荷塘 위로 올 리 기에 무수히 굴 곡진 弥望 의 田田 의 잎 은.\
					 잎 이 매우 높 은 것 으로 亭亭 수면 위로 드 러 난 처 럼 의 무희 의 치마 를 걷 어 올 려 했 다.\
					 층층이 의 잎 을 장식 하 고 중간 산발적 인 꽃 이 袅娜 좀 하 게 열 려 있다 는 수 줍 게 인사 를 전 했 다 朵儿;\
					 톨 한 톨 의 맹주 를 또 碧天 하 고 또 갓 出浴 의 미인 이다.나 로 우주 센터 에 보 내 缕缕 적이 곳, \
					 열심히 마치 멀리 고층 에서 막 연한 노래 를 놓 는 것 과 같다.이 럴 때 잎 과 꽃 도 에 한 가닥 의 번개 처 럼 흔 들 리 고 있 었 지만,\
					 그 는 荷塘 쪽 으로 가 주 세요.잎 본 은 어깨동무 를 하 고 密密地 으라고, 이 완연히 있 는 양자택일 응결되다 벽 파 흔적 이다.\
					 잎 은 갈무리 의 물 밑 으로 가 렸 을 못 만 나 일부 색깔;반면 잎 은 훨씬 더 만날 풍치 했 다.";

const static std::wstring insert_korean = L"INSERT INTO `t_korean` (`context`) VALUES ('한 荷塘 위로 올 리 기에 무수히 굴 곡진 弥望 의 田田 의 잎 은.잎 이 매우 높 은 것 으로 亭亭 수면 위로 드 러 난 처 럼 의 무희 의 치마 를 걷 어 올 려 했 다.층층이 의 잎 을 장식 하 고 중간 산발적 인 꽃 이 袅娜 좀 하 게 열 려 있다 는 수 줍 게 인사 를 전 했 다 朵儿;톨 한 톨 의 맹주 를 또 碧天 하 고 또 갓 出浴 의 미인 이다.나 로 우주 센터 에 보 내 缕缕 적이 곳, 열심히 마치 멀리 고층 에서 막 연한 노래 를 놓 는 것 과 같다.이 럴 때 잎 과 꽃 도 에 한 가닥 의 번개 처 럼 흔 들 리 고 있 었 지만, 그 는 荷塘 쪽 으로 가 주 세요.잎 본 은 어깨동무 를 하 고 密密地 으라고, 이 완연히 있 는 양자택일 응결되다 벽 파 흔적 이다.잎 은 갈무리 의 물 밑 으로 가 렸 을 못 만 나 일부 색깔;반면 잎 은 훨씬 더 만날 풍치 했 다.')";

enum LanguageDatabaseStatements
{
    /*  Naming standard for defines:
        {DB}_{SET/DEL/ADD/REP}_{Summary of data changed}
        When updating more than one field, consider looking at the calling function
        name for a suiting suffix.
    */
	LANGUAGE_ADD_CHINESE,
	LANGUAGE_ADD_ENGLISH,
	LANGUAGE_ADD_JANPANESE,
	LANGUAGE_ADD_RUSSIAN,
	LANGUAGE_ADD_KOREAN,
	LANGUAGE_SELECT_CHINESE,
	LANGUAGE_VAGUE_QUERY_CHINESE,
	LANGUAGE_SELECT_ENGLISH,
	LANGUAGE_VAGUE_QUERY_ENGLISH,
	LANGUAGE_SELECT_JANPANESE,
	LANGUAGE_VAGUE_QUERY_JANPANESE,
	LANGUAGE_SELECT_RUSSIAN,
	LANGUAGE_VAGUE_QUERY_RUSSIAN,
	LANGUAGE_SELECT_KOREAN,
	LANGUAGE_VAGUE_QUERY_KOREAN,
	LANGUAGE_UPDATE,
	LANGUAGE_DEL,
    MAX_LANGUAGE_DATABASE_STATEMENTS,
};

class LanguageDatabaseConnection : public easy::EasyMySQLConnection
{
public:
	//- Constructors for sync and async connections
	LanguageDatabaseConnection(easy::EasyMySQLConnectionInfo& conn_info) : easy::EasyMySQLConnection(conn_info) { }

	//- Loads database type specific prepared statements
	void DoPrepareStatements() 
	{
		if (!reconnecting_)
			mysql_stmts_.resize(MAX_LANGUAGE_DATABASE_STATEMENTS);
			PREPARE_STATEMENT(LANGUAGE_ADD_CHINESE,  "INSERT INTO t_chinese(context) VALUES(?)" , easy::CONNECTION_SYNCH)
			PREPARE_STATEMENT(LANGUAGE_ADD_ENGLISH,  "INSERT INTO t_english(context) VALUES(?)" , easy::CONNECTION_SYNCH)
			PREPARE_STATEMENT(LANGUAGE_ADD_JANPANESE,  "INSERT INTO t_japanese(context) VALUES(?)" , easy::CONNECTION_SYNCH)
			PREPARE_STATEMENT(LANGUAGE_ADD_RUSSIAN,  "INSERT INTO t_russian(context) VALUES(?)" , easy::CONNECTION_SYNCH)
			PREPARE_STATEMENT(LANGUAGE_ADD_KOREAN,  "INSERT INTO t_korean(context) VALUES(?)" , easy::CONNECTION_SYNCH)
			PREPARE_STATEMENT(LANGUAGE_SELECT_CHINESE,  "SELECT context FROM t_chinese where id = ?" , easy::CONNECTION_SYNCH)
			PREPARE_STATEMENT(LANGUAGE_VAGUE_QUERY_CHINESE,  "SELECT context FROM t_chinese where context like '%叶子%'" , easy::CONNECTION_SYNCH)
			PREPARE_STATEMENT(LANGUAGE_SELECT_ENGLISH,  "SELECT context FROM t_english where id = ?" , easy::CONNECTION_SYNCH)
			PREPARE_STATEMENT(LANGUAGE_VAGUE_QUERY_ENGLISH,  "SELECT context FROM t_english where context like '%leaves%'" , easy::CONNECTION_SYNCH)
			PREPARE_STATEMENT(LANGUAGE_SELECT_JANPANESE,  "SELECT context FROM t_japanese where id = ?" , easy::CONNECTION_SYNCH)
			PREPARE_STATEMENT(LANGUAGE_VAGUE_QUERY_JANPANESE,  "SELECT context FROM t_japanese where context like '%葉%'" , easy::CONNECTION_SYNCH)
			PREPARE_STATEMENT(LANGUAGE_SELECT_RUSSIAN,  "SELECT context FROM t_russian where id = ?" , easy::CONNECTION_SYNCH)
			PREPARE_STATEMENT(LANGUAGE_VAGUE_QUERY_RUSSIAN,  "SELECT context FROM t_russian where context like '%лист%'" , easy::CONNECTION_SYNCH)
			PREPARE_STATEMENT(LANGUAGE_SELECT_KOREAN,  "SELECT context FROM t_korean where id = ?" , easy::CONNECTION_SYNCH)
			PREPARE_STATEMENT(LANGUAGE_VAGUE_QUERY_KOREAN,  "SELECT context FROM t_korean where context like '%잎%'" , easy::CONNECTION_SYNCH)
			PREPARE_STATEMENT(LANGUAGE_UPDATE,  "UPDATE t_chinese SET context = ? WHERE id = ?" , easy::CONNECTION_SYNCH)
			PREPARE_STATEMENT(LANGUAGE_DEL,  "DELETE FROM t_chinese WHERE id = ?" , easy::CONNECTION_SYNCH)
	}
};

typedef easy::EasyDatabaseWorkerPool<LanguageDatabaseConnection> LanguageDatabaseWorkerPool;

typedef std::string String;
typedef std::wstring WString;

WString UTF8ToUnicode( const String& sValue )
{
	size_t len = 0;
	len = sValue.length();
	int unicodeLen = ::MultiByteToWideChar( CP_UTF8, 0, sValue.c_str(), -1, NULL, 0 ); 
	wchar_t * pUnicode; 
	pUnicode = new wchar_t[unicodeLen+1]; 
	memset(pUnicode,0,(unicodeLen+1)*sizeof(wchar_t)); 
	::MultiByteToWideChar( CP_UTF8, 0, sValue.c_str(), -1, (LPWSTR)pUnicode, unicodeLen ); 
	WString rt; 
	rt = ( wchar_t* )pUnicode;
	delete pUnicode; 

	return rt; 
}

//--------------------------------------------------------------------
String UnicodeToUTF8( const WString& wsValue )
{
	char*     pElementText;
	int    iTextLen;
	// wide char to multi char
	iTextLen = ::WideCharToMultiByte( CP_UTF8, 0, wsValue.c_str(), -1, NULL, 0, NULL, NULL );
	pElementText = new char[iTextLen + 1];
	memset( ( void* )pElementText, 0, sizeof( char ) * ( iTextLen + 1 ) );
	::WideCharToMultiByte( CP_UTF8, 0, wsValue.c_str(), -1, pElementText, iTextLen, NULL, NULL );
	String strText;
	strText = pElementText;
	delete[] pElementText;
	return strText;
}

String UnicodeToAnsi( const WString& wsValue )
{
	char* pElementText;
	int    iTextLen;
	// wide char to multi char
	iTextLen = WideCharToMultiByte( CP_ACP, 0, wsValue.c_str(), -1, NULL, 0, NULL, NULL );
	pElementText = new char[iTextLen + 1];
	memset( ( void* )pElementText, 0, sizeof( char ) * ( iTextLen + 1 ) );
	::WideCharToMultiByte( CP_ACP, 0, wsValue.c_str(), -1, pElementText, iTextLen, NULL, NULL );
	String strText = pElementText;
	delete[] pElementText;
	return strText;
}

//
// TestCase class
//
class TestLanguageDatabase : public CPPUNIT_NS::TestCase
{
	CPPUNIT_TEST_SUITE(TestLanguageDatabase);
#if 0
	CPPUNIT_IGNORE;
#endif
	CPPUNIT_TEST(test);
	CPPUNIT_TEST_SUITE_END();

protected:
	void test();
};

CPPUNIT_TEST_SUITE_REGISTRATION(TestLanguageDatabase);

//
// tests implementation
//
using namespace easy;
void TestLanguageDatabase::test()
{
	
	LanguageDatabaseWorkerPool	language_database;
	std::string database_info = "localhost;3306;root;sbivfh;language_database";
	language_database.Open(database_info,0,1);
	easy::EasyPreparedStatement* pre_stmt = NULL;

	// insert all kinds of context

	//////////////////////////////////////////////////////////////////////////
#if 1
	pre_stmt = language_database.GetPreparedStatement(LANGUAGE_ADD_CHINESE);
	pre_stmt->setString(0,UnicodeToUTF8(chinese));
	language_database.DirectExecute(pre_stmt);

	pre_stmt = NULL;
	pre_stmt = language_database.GetPreparedStatement(LANGUAGE_ADD_ENGLISH);
	pre_stmt->setString(0,UnicodeToUTF8(english));
	language_database.DirectExecute(pre_stmt);

	pre_stmt = NULL;
	pre_stmt = language_database.GetPreparedStatement(LANGUAGE_ADD_JANPANESE);
	pre_stmt->setString(0,UnicodeToUTF8(janpanese));
	language_database.DirectExecute(pre_stmt);

	pre_stmt = NULL;
	pre_stmt = language_database.GetPreparedStatement(LANGUAGE_ADD_RUSSIAN);
	pre_stmt->setString(0,UnicodeToUTF8(russian));
	language_database.DirectExecute(pre_stmt);
#endif

#if 0
	pre_stmt = NULL;
	pre_stmt = language_database.GetPreparedStatement(LANGUAGE_ADD_KOREAN);
	pre_stmt->setString(0,UnicodeToUTF8(korean));
	language_database.DirectExecute(pre_stmt);
#else
	//	just insert context
		language_database.Execute(UnicodeToUTF8(insert_korean).c_str());
#endif

	//////////////////////////////////////////////////////////////////////////
#if 1
	pre_stmt = language_database.GetPreparedStatement(LANGUAGE_SELECT_CHINESE);
	pre_stmt->setInt32(0,1);
#else
	pre_stmt = language_database.GetPreparedStatement(LANGUAGE_VAGUE_QUERY_CHINESE);
#endif
	easy::PreparedQueryResult pre_result_result_chinese = language_database.Query(pre_stmt);
	if (pre_result_result_chinese)
	{
		do 
		{
			easy::EasyField* fields = pre_result_result_chinese->Fetch();
			std::string context = UnicodeToAnsi(UTF8ToUnicode(fields[0].GetString()));
			printf("%s\n",context.c_str());
		} while (pre_result_result_chinese->NextRow());
	}

#if 1
	pre_stmt = language_database.GetPreparedStatement(LANGUAGE_SELECT_ENGLISH);
	pre_stmt->setInt32(0,1);
#else
	pre_stmt = language_database.GetPreparedStatement(LANGUAGE_VAGUE_QUERY_ENGLISH);
#endif
	easy::PreparedQueryResult pre_result_result_english = language_database.Query(pre_stmt);
	if (pre_result_result_english)
	{
		do 
		{
			easy::EasyField* fields = pre_result_result_english->Fetch();
			std::string context = UnicodeToAnsi(UTF8ToUnicode(fields[0].GetString()));
			printf("%s\n",context.c_str());
		} while (pre_result_result_english->NextRow());
	}

#if 1
	pre_stmt = language_database.GetPreparedStatement(LANGUAGE_SELECT_JANPANESE);
	pre_stmt->setInt32(0,1);
#else
	pre_stmt = language_database.GetPreparedStatement(LANGUAGE_VAGUE_QUERY_JANPANESE);
#endif
	easy::PreparedQueryResult pre_result_result_japanese = language_database.Query(pre_stmt);
	if (pre_result_result_japanese)
	{
		do 
		{
			easy::EasyField* fields = pre_result_result_japanese->Fetch();
			std::string context = UnicodeToAnsi(UTF8ToUnicode(fields[0].GetString()));
			printf("%s\n",context.c_str());
		} while (pre_result_result_japanese->NextRow());
	}

#if 1
	pre_stmt = language_database.GetPreparedStatement(LANGUAGE_SELECT_RUSSIAN);
	pre_stmt->setInt32(0,1);
#else
	pre_stmt = language_database.GetPreparedStatement(LANGUAGE_VAGUE_QUERY_RUSSIAN);
#endif
	easy::PreparedQueryResult pre_result_result_russian = language_database.Query(pre_stmt);
	if (pre_result_result_russian)
	{
		do 
		{
			easy::EasyField* fields = pre_result_result_russian->Fetch();
			std::string context = UnicodeToAnsi(UTF8ToUnicode(fields[0].GetString()));
			printf("%s\n",context.c_str());
		} while (pre_result_result_russian->NextRow());
	}

#if 1
	pre_stmt = language_database.GetPreparedStatement(LANGUAGE_SELECT_KOREAN);
	pre_stmt->setInt32(0,1);
#else
	pre_stmt = language_database.GetPreparedStatement(LANGUAGE_VAGUE_QUERY_KOREAN);
#endif
	easy::PreparedQueryResult pre_result_result_korean = language_database.Query(pre_stmt);
	if (pre_result_result_korean)
	{
		do 
		{
			easy::EasyField* fields = pre_result_result_korean->Fetch();
			std::wstring context_unicode = UTF8ToUnicode(fields[0].GetString());
			std::string context_ascii = UnicodeToAnsi(context_unicode);
			printf("%s\n",context_ascii.c_str());
		} while (pre_result_result_korean->NextRow());
	}
}
