#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <regex>
#include <algorithm>

using namespace std;

class CProcessUrl
{
public:
	CProcessUrl();
	~CProcessUrl();

public:
	//returns 'true', if name had been written;
	bool SetInputFileName(const string&) noexcept;

	//returns 'true', if name had been written;
	bool SetOutputFileName(const string&) noexcept;

	//returns 'true', if searching has been finished
	//returns 'false' in other cases
	bool StartOfProcessing() noexcept;

	//get states of boolean flags
	vector<bool> ReturnStateVector() noexcept;

private:
	bool _IsThereUrlInStr(const string&) noexcept;

	//if files can be opened, returns 'true' and creates streams
	bool _IsValidNames() noexcept;

	void _SetStreamError() noexcept;

	void _SetSuccessState() noexcept;

	bool _CheckInputStream() noexcept;

	bool _CheckOutputStream() noexcept;

	void _ReadText() noexcept;

	string _CutFirstSent(const int&) noexcept;

	int _FindEndSent() noexcept;

	void _ProcessText() noexcept;

	void _WriteText() noexcept;

private:
	bool m_bIsInitInputFileName;
	bool m_bIsTextRead;
	bool m_bIsInitOutputFileName;
	bool m_bIsTextProcessed;
	bool m_bIsOpenInputFile;
	bool m_bIsOpenOutputFile;
	bool m_bIsTextWritten;
	bool m_bIsFinishedProcess;
	bool m_bIsStreamInterrupted;

	bool bDotFlag;
	char cSymbol;
	string strSentense;

	string strText;

	string m_strInputFileName;
	string m_strOutputFileName;

	vector<string> m_vecResult;

	ifstream m_fileInput;
	ofstream m_fileOutput;

private:
	const char _POINT_CHAR = '.';
	const char _SPACE_CHAR = ' ';
	const char _ENTER_CHAR = '\n';
	const char _TAB_CHAR = '\t';
	const int _STEP_FROM_SENT_END = 1;	//last punctuation mark and space
	const int _BEGIN_STR = 0;
	const int _END_SENT_STEP = 2;
	const int _CUT_STEP = 1;
	const int _MIN_POS = 0;
	const vector<string> _ENDS_OF_SENT = { ". ","! ","? "};
	const char _DEFAULT_CHAR_VALUE = '-';
	const bool _FAIL = false;
	const bool _CLOSED = false;
	const bool _SUCCESS = true;
	const vector<string> _REGULAR_EXPRESSIONS = {"[\\w]{2,}\\.[a-z]{2,5}",	//softtech.com
												 "www\\.[\\w]{2,}\\.[a-z]{2,5}",	//www.softtech.com 
												 "http:\\/\\/[\\w]{2,}\\.[a-z]{2,5}",	//http://softtech.com
												 "https:\\/\\/[\\w]{2,}\\.[a-z]{2,5}",//https://softtech.com 
												 "http:\\/\\/www\\.[\\w]{2,}\\.[a-z]{2,5}",//http://www.softech.com
												 "https:\\/\\/www\\.[\\w]{2,}\\.[a-z]{2,5}"//https://www.softech.com
												};
	const string _DEFAULT_STR_VALUE = "";
};