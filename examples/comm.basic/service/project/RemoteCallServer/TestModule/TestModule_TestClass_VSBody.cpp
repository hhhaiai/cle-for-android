/*--------------------------------------------------*/
/*VirtualSociety System ServiceModuleTemplate Main File*/
/*CreateBy SRPLab                */
/*CreateDate: 2010-11-15  */
/*--------------------------------------------------*/
#include "RemoteCallServer_VSHeader.H"

VS_INT32 SRPAPI TestClass_GetNumber(void *Object,VS_INT32 Para)
{
	pSRP -> Print( "Remote Call Number [%d][%d]",Object,Para);
	return Para+1;
}

VS_FLOAT SRPAPI TestClass_GetNumberF(void *Object,VS_FLOAT Para)
{
	pSRP -> Print( "Remote Call Number [%d][%f]",Object,Para);
	return Para+111.111;
}

VS_CHAR * SRPAPI TestClass_GetString(void *Object,VS_CHAR * Para)
{
	static VS_CHAR CharBuf[128];

    pSRP -> Print( "Remote Call String [%s]",Para);
	sprintf(CharBuf,"%sasdfsaf",Para);
    return CharBuf;
}

VS_PARAPKGPTR ParaPkgPtr = NULL;

VS_PARAPKGPTR SRPAPI TestClass_GetPkg(void *Object,VS_PARAPKGPTR Para)
{
	if( ParaPkgPtr == NULL )
		ParaPkgPtr = pSRP -> GetParaPkgInterface();
    pSRP -> Print( "Remote Call Pkg [%d]",Para ->GetInt(0));
	ParaPkgPtr ->Clear();
	ParaPkgPtr ->InsertStr(0,"asdfsaf");
    return ParaPkgPtr;
}
