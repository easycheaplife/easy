/********************************************************************
	created:	2011/09/20
	created:	20:9:2011   14:48
	filename: 	d:\Sever_Redeploy\iwserver\src\RoleServer\exception_define.h
	file path:	d:\Sever_Redeploy\iwserver\src\RoleServer
	file base:	exception_define
	file ext:	h
	author:		slim shady
	
	purpose:	helper macros
*********************************************************************/
#ifndef exception_define_h__
#define exception_define_h__
#include "stdafx.h"
#include <stdio.h>

#define  USE_LOG
#if	 defined(__FUNCTION__) && defined(USE_TRACE)
	#define SLIM_TRACE	printf_s("enter function: %s",__FUNCTION__);
#else
	#define SLIM_TRACE
#endif	// end of USE_TRACE

#if	 defined(USE_LOG) && defined(_DEBUG)
	#define SLIM_LOG1(__szFormat,__var1)						printf_s(__szFormat,__var1);
	#define SLIM_LOG2(__szFormat,__var1,__var2)					printf_s(__szFormat,__var1,__var2);
	#define SLIM_LOG3(__szFormat,__var1,__var2,__var3)			printf_s(__szFormat,__var1,__var2,__var3);
	#define SLIM_LOG4(__szFormat,__var1,__var2,__var3,__var4)	printf_s(__szFormat,__var1,__var2,__var3,__var4);
#else
	#define SLIM_LOG1(__szFormat,__var1)
	#define SLIM_LOG2(__szFormat,__var1,__var2)
	#define SLIM_LOG3(__szFormat,__var1,__var2,__var3)
	#define SLIM_LOG4(__szFormat,__var1,__var2,__var3,__var4)
#endif	// end of USE_LOG


#endif // exception_define_h__