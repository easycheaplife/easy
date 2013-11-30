#include "easy_cppunit_proxy.h"
#include "corpus_proxy.h"
#include <string.h> 
//
// TestCase class
//
class TestCorpusProxy : public CPPUNIT_NS::TestCase
{
	CPPUNIT_TEST_SUITE(TestCorpusProxy);
#if 0
	CPPUNIT_IGNORE;
#endif
	CPPUNIT_TEST(test);
	CPPUNIT_TEST_SUITE_END();

protected:
	void test();
};

CPPUNIT_TEST_SUITE_REGISTRATION(TestCorpusProxy);

//
// tests implementation
//

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

void TestCorpusProxy::test()
{
	CorpusProxy proxy;
	bool success = proxy.Connect("127.0.0.1","root","sbivfh");
	if (!success)
	{
		std::cout << "connect mysql failed!" << std::endl;
		return;
	}
	proxy.AddUser("lee","lee",PERMISSION_DWONLOAD);
	VisitPermission permission = proxy.Login("lee","lee");

	std::wstring unicode_file_name(L"file_name");
	std::string ftp_path("ftp://lee:123456@127.0.0.1:9876");
	int	character_count = 50000;
	
#if 1
	proxy.Upload(CORPUS_CHINESE,unicode_file_name,chinese,ftp_path,wcslen(chinese.c_str()));
	proxy.Upload(CORPUS_ENGLISH,unicode_file_name,english,ftp_path,wcslen(english.c_str()));
	proxy.Upload(CORPUS_JAPANESE,unicode_file_name,janpanese,ftp_path,wcslen(janpanese.c_str()));
	proxy.Upload(CORPUS_RUSSIAN,unicode_file_name,russian,ftp_path,wcslen(russian.c_str()));
	proxy.Upload(CORPUS_KOREAN,unicode_file_name,korean,ftp_path,wcslen(korean.c_str()));

	proxy.Delete(CORPUS_CHINESE,unicode_file_name);
	proxy.Delete(CORPUS_ENGLISH,unicode_file_name);
	proxy.Delete(CORPUS_JAPANESE,unicode_file_name);
	proxy.Delete(CORPUS_RUSSIAN,unicode_file_name);
	proxy.Delete(CORPUS_KOREAN,unicode_file_name);
#endif

	std::string file_name;
	std::wstring sel_chinese_key_word = L"叶";
	std::map<std::string,context_info*>	__map_context_info;
	list_context_info __lock_queue;

	unsigned int __count_chinese = proxy.Count(CORPUS_CHINESE,sel_chinese_key_word);
	unsigned int __record_count_chinese = proxy.Count(CORPUS_CHINESE);
	unsigned int __character_count_chinese = proxy.Character_Count(CORPUS_CHINESE);
	unsigned int __character_count_english = proxy.Character_Count(CORPUS_ENGLISH);

	proxy.Search(CORPUS_CHINESE,sel_chinese_key_word,__lock_queue);
	for(list_context_info::iterator __it = __lock_queue.begin(); __it != __lock_queue.end(); ++__it)
	{
		context_info* __context_info = (*__it);
		if (__context_info)
		{
			std::cout << "CORPUS_CHINESE KEYWORD:" << __context_info->character_count_ << __context_info->ftp_path_ << proxy.UnicodeToAnsi(__context_info->file_name_) << proxy.UnicodeToAnsi(__context_info->full_context_) << std::endl;
			delete __context_info;
			__context_info = NULL;
		}
	}
	__lock_queue.clear();

	//	select limit
	proxy.Search(CORPUS_CHINESE,sel_chinese_key_word,__lock_queue,0,4);
	for(list_context_info::iterator __it = __lock_queue.begin(); __it != __lock_queue.end(); ++__it)
	{
		context_info* __context_info = (*__it);
		if (__context_info)
		{
			std::cout << "CORPUS_CHINESE KEYWORD LIMIT:" << __context_info->character_count_ << __context_info->ftp_path_ << proxy.UnicodeToAnsi(__context_info->file_name_) << proxy.UnicodeToAnsi(__context_info->full_context_) << std::endl;
			delete __context_info;
			__context_info = NULL;
		}
	}
	__lock_queue.clear();

	std::wstring sel_chinese_word = L"叶子";
	proxy.Search(CORPUS_CHINESE,sel_chinese_word,__lock_queue);
	for(list_context_info::iterator __it = __lock_queue.begin(); __it != __lock_queue.end(); ++__it)
	{
		context_info* __context_info =  (*__it);
		if (__context_info)
		{
			std::cout << "CORPUS_CHINESE WORD:" << __context_info->character_count_ << __context_info->ftp_path_ << proxy.UnicodeToAnsi(__context_info->file_name_) << proxy.UnicodeToAnsi(__context_info->full_context_) << std::endl;
			delete __context_info;
			__context_info = NULL;
		}
	}
	__lock_queue.clear();

	std::wstring sel_chinese_sentence = L"弥望的是田田的叶子";
	proxy.Search(CORPUS_CHINESE,sel_chinese_sentence,__lock_queue);
	for(list_context_info::iterator __it = __lock_queue.begin(); __it != __lock_queue.end(); ++__it)
	{
		context_info* __context_info =  (*__it);
		if (__context_info)
		{
			std::cout << "CORPUS_CHINESE SEBTENCE:" << __context_info->character_count_ << __context_info->ftp_path_ << proxy.UnicodeToAnsi(__context_info->file_name_) << proxy.UnicodeToAnsi(__context_info->full_context_) << std::endl;
			delete __context_info;
			__context_info = NULL;
		}
	}
	__lock_queue.clear();

	std::wstring sel_english_key_word = L"leaves";
	proxy.Search(CORPUS_ENGLISH,sel_english_key_word,__lock_queue);
	for(list_context_info::iterator __it = __lock_queue.begin(); __it != __lock_queue.end(); ++__it)
	{
		context_info* __context_info =  (*__it);
		if (__context_info)
		{
			std::cout << "CORPUS_ENGLISH KEYWORD:" << __context_info->character_count_ << __context_info->ftp_path_ << proxy.UnicodeToAnsi(__context_info->file_name_) << proxy.UnicodeToAnsi(__context_info->full_context_) << std::endl;
			delete __context_info;
			__context_info = NULL;
		}
	}
	__lock_queue.clear();

	std::wstring sel_english_word = L"above the water";
	proxy.Search(CORPUS_ENGLISH,sel_english_word,__lock_queue);
	for(list_context_info::iterator __it = __lock_queue.begin(); __it != __lock_queue.end(); ++__it)
	{
		context_info* __context_info =  (*__it);
		if (__context_info)
		{
			std::cout << "CORPUS_ENGLISH WORD:" << __context_info->character_count_ << __context_info->ftp_path_ << proxy.UnicodeToAnsi(__context_info->file_name_) << proxy.UnicodeToAnsi(__context_info->full_context_) << std::endl;
			delete __context_info;
			__context_info = NULL;
		}
	}
	__lock_queue.clear();

	std::wstring sel_english_sentence = L"A breeze stirs, sending over breaths fragrance, like the drifting from a distant building singing";
	proxy.Search(CORPUS_ENGLISH,sel_english_sentence,__lock_queue);
	for(list_context_info::iterator __it = __lock_queue.begin(); __it != __lock_queue.end(); ++__it)
	{
		context_info* __context_info =  (*__it);
		if (__context_info)
		{
			std::cout << "CORPUS_ENGLISH SENTENCE::" << __context_info->character_count_ << __context_info->ftp_path_ << proxy.UnicodeToAnsi(__context_info->file_name_) << proxy.UnicodeToAnsi(__context_info->full_context_) << std::endl;
			delete __context_info;
			__context_info = NULL;
		}
	}
	__lock_queue.clear();
}
