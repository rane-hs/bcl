/*! bclxml.h
	xmlの読み書きについて、マルチプラットフォーム対応させるためのヘッダファイル
	VC, MinGW(with binutils)に対応済
*/


#ifndef __BCL_XML_H__
#define __BCL_XML_H__

#include <string>
#include <bcl/bclstr.h>
//MSXMLを使用するためのインポート
#ifdef _MSC_VER
#	import "msxml3.dll"

#else
#	include "msxml2.h"

#endif

namespace bcl{
	
class com_init{
public:
	com_init(){ CoInitialize(NULL);}
	~com_init(){ CoUninitialize();}
};
#ifdef _MSC_VER
typedef MSXML2::IXMLDOMDocumentPtr xmlDocumentPtr;
typedef MSXML2::IXMLDOMNodePtr xmlNodePtr;
typedef MSXML2::IXMLDOMNodePtr xmlAttributePtr;
typedef MSXML2::IXMLDOMNodeListPtr xmlNodeListPtr;
inline const xmlNodePtr GetNode(xmlDocumentPtr pDoc, const std::string &xPath)
{	return (pDoc)?
		pDoc->selectSingleNode(xPath.c_str()) :
		NULL;
}
inline const xmlAttributePtr GetAttr(xmlDocumentPtr pDoc, const std::string &xPath)
{	return (pDoc)?
		pDoc->selectSingleNode(xPath.c_str()) :
		NULL;
}
inline const IXMLDOMNodeListPtr GetNodeList(xmlDocumentPtr pDoc, const std::string &xPath)
{	return (pDoc)?
		pDoc->selectNodes(xPath.c_str()) :
		NULL;
}
#ifdef _UNICODE
inline const std::wstring NodeTextW(xmlNodePtr pNode)
{	BSTR bstrdest;
	pNode->get_text(&bstrdest);
	std::wstring dest(bstrdest);
	SysFreeString(bstrdest);
	return dest;
}
inline const std::string NodeText(xmlNodePtr pNode)
{	return bcl::narrow(NodeTextW(pNode));
}
#else
inline const std::string NodeText(xmlNodePtr pNode)
{	return static_cast<LPCTSTR>(pNode->text);
}
#endif

inline const xmlNodePtr NextNode(xmlNodeListPtr &pList)
{	return pList->nextNode();
}
inline xmlDocumentPtr XMLdocument()
{
	xmlDocumentPtr pDoc;
	
	//インスタンスの作成
	HRESULT hr = pDoc.CreateInstance(__uuidof(MSXML2::DOMDocument30));
	if( FAILED(hr) ) 
		return NULL;
	//ＸＭＬファイルのロードが終わるまで待つモード
	pDoc->async = VARIANT_FALSE;
	return pDoc;
};
inline xmlDocumentPtr XMLdocument(const std::string fpath)
{
	//ＸＭＬファイルのロード
	xmlDocumentPtr pDoc = XMLdocument();
	if (pDoc != NULL) {
		pDoc->validateOnParse = VARIANT_FALSE;
		pDoc->resolveExternals = VARIANT_FALSE;
		if (pDoc->load(fpath.c_str()) == VARIANT_FALSE) {
			//printf("%s\n", (const char *)pDoc->parseError->Getreason());
			return NULL;
		}
	}
	return pDoc;
};
inline xmlDocumentPtr XMLdocument(const std::wstring fpath)
{
	//ＸＭＬファイルのロード
	xmlDocumentPtr pDoc = XMLdocument();
	if (pDoc != NULL) {
		pDoc->validateOnParse = VARIANT_FALSE;
		pDoc->resolveExternals = VARIANT_FALSE;
		if (pDoc->load(fpath.c_str()) == VARIANT_FALSE) {
			//printf("%s\n", (const char *)pDoc->parseError->Getreason());
			return NULL;
		}
	}
	return pDoc;
};
inline xmlDocumentPtr XMLdocument(_bstr_t bstrXml)
{
	// ＸＭＬデータのロード
	xmlDocumentPtr pDoc = XMLdocument();
	if (pDoc != NULL) {
		pDoc->validateOnParse = VARIANT_FALSE;
		pDoc->resolveExternals = VARIANT_FALSE;
		if (pDoc->loadXML(bstrXml) == VARIANT_FALSE) {
			return NULL;
		}
	}
	return pDoc;
};
#else	//MSC_VER

typedef IXMLDOMDocument* xmlDocumentPtr;
typedef IXMLDOMNode* xmlNodePtr;
typedef IXMLDOMNode* xmlAttributePtr;
typedef IXMLDOMNodeList* xmlNodeListPtr;

inline const xmlNodePtr GetNode(xmlDocumentPtr &pDoc, const std::string &xPath)
{	if(pDoc==NULL) return NULL;
	xmlNodePtr	pDest;
	HRESULT hr = pDoc->selectSingleNode(const_cast<wchar_t*>(widen(xPath).c_str()), &pDest);
	if( FAILED(hr) ) return NULL;
	return pDest;
}
inline const xmlNodePtr GetNode(xmlNodePtr &pNode, const std::string &xPath)
{	if(pNode==NULL) return NULL;
	xmlNodePtr	pDest;
	HRESULT hr = pNode->selectSingleNode(const_cast<wchar_t*>(widen(xPath).c_str()), &pDest);
	if( FAILED(hr) ) return NULL;
	return pDest;
}
inline const xmlAttributePtr GetAttr(xmlNodePtr &pDoc, const std::string &xPath)
{	if(pDoc==NULL) return NULL;
	xmlNodePtr	pDest;
	HRESULT hr = pDoc->selectSingleNode(const_cast<wchar_t*>(widen(xPath).c_str()), &pDest);
	if( FAILED(hr) ) return NULL;
	return pDest;
}
inline const xmlNodeListPtr GetNodeList(xmlNodePtr &pDoc, const std::string &xPath)
{	if(pDoc==NULL) return NULL;
	xmlNodeListPtr	pDest;
	HRESULT hr = pDoc->selectNodes(const_cast<wchar_t*>(widen(xPath).c_str()), &pDest);
	if( FAILED(hr) ) return NULL;
	return pDest;
}
inline const std::string NodeText(xmlNodePtr &pNode)
{	if(pNode==NULL) return "";
	BSTR bDest;
	pNode->get_text(&bDest);
	const std::string dest = bcl::narrow(bDest);
	SysFreeString(bDest);
	return dest;
}
inline const xmlNodePtr NextNode(xmlNodeListPtr &pList)
{	xmlNodePtr	pDest;
	pList->nextNode(&pDest);
	return pDest;
}

inline xmlDocumentPtr XMLdocument()
{
	xmlDocumentPtr pDoc;
	
	//インスタンスの作成
	HRESULT hr = ::CoCreateInstance(CLSID_DOMDocument40, 
		NULL, 
		CLSCTX_INPROC_SERVER, 
		IID_IXMLDOMDocument2,
		(void**)&pDoc);

	if( FAILED(hr) ) return NULL;
	//ＸＭＬファイルのロードが終わるまで待つモード
	hr = pDoc->put_async(VARIANT_FALSE);
	if (FAILED(hr)) return NULL;

	return pDoc;
};
inline xmlDocumentPtr XMLdocument(const std::wstring &fpath)
{
	//ＸＭＬファイルのロード
	xmlDocumentPtr pDoc = XMLdocument();
	if (pDoc == NULL) return NULL;
	
	pDoc->put_validateOnParse(VARIANT_FALSE);
	pDoc->put_resolveExternals(VARIANT_FALSE);
	VARIANT_BOOL	status;
	VARIANT variantFile;
	variantFile.bstrVal = SysAllocString(fpath.c_str());
	V_VT(&variantFile) = VT_BSTR;

	HRESULT hr = pDoc->load(variantFile, &status);
	if (&variantFile)	VariantClear(&variantFile);
	
	if (status != VARIANT_TRUE) {
		//printf("%s\n", (const char *)pDoc->parseError->Getreason());
		return NULL;
	}
	return pDoc;
};
inline xmlDocumentPtr XMLdocument(const std::string &fpath)
{
	return XMLdocument(bcl::widen(fpath));
}
#endif

}; //namespace bcl{

#endif //__BCL_XML_H__
