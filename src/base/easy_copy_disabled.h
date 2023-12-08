/********************************************************************
	created:	2011/09/25
	created:	25:9:2011   11:54
	filename: 	F:\easy\src\base\easy_copy_disabled.h
	file path:	F:\easy\src\base
	file base:	easy_copy_disabled
	file ext:	h
	author:		Lee

	purpose:
*********************************************************************/
#ifndef easy_copy_disabled_h__
#define easy_copy_disabled_h__

namespace easy {
class EasyCopyDisabled {
  public:
    //	default constructor
    EasyCopyDisabled (void) { }
  private:
    EasyCopyDisabled ( const EasyCopyDisabled& );
    EasyCopyDisabled& operator = ( const EasyCopyDisabled& );
};
}

#endif // easy_copy_disabled_h__