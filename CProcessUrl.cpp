#include "CProcessUrl.h"

CProcessUrl::CProcessUrl()
{
	m_bIsInitInputFileName = false;
	m_bIsInitOutputFileName = false;
	m_bIsFinishedProcess = false;
	m_bIsStreamInterrupted = false;
	m_bIsTextRead = false;
	m_bIsOpenInputFile = false;
	m_bIsOpenOutputFile = false;
	m_bIsTextProcessed = false;
	m_bIsTextWritten = false;

	m_strInputFileName = _DEFAULT_STR_VALUE;
	m_strOutputFileName = _DEFAULT_STR_VALUE;
}

CProcessUrl::~CProcessUrl()
{
	m_fileInput.close();
	m_fileOutput.close();
}

bool CProcessUrl::SetInputFileName(const string& STR_FILE_NAME) noexcept
{
	_ASSERT(!STR_FILE_NAME.empty());
	m_strInputFileName = STR_FILE_NAME;
	if (m_strInputFileName == STR_FILE_NAME)
		m_bIsInitInputFileName = _SUCCESS;
	
	return m_bIsInitInputFileName;
}

bool CProcessUrl::SetOutputFileName(const string& STR_FILE_NAME) noexcept
{
	_ASSERT(!STR_FILE_NAME.empty());
	m_strOutputFileName = STR_FILE_NAME;
	if (m_strOutputFileName == STR_FILE_NAME)
		m_bIsInitOutputFileName = _SUCCESS;

	return m_bIsInitOutputFileName;
}

bool CProcessUrl::_IsThereUrlInStr(const string& STR_SENTENSE) noexcept
{
	_ASSERT(!STR_SENTENSE.empty());
	bool bResult = false;
	for (auto i : _REGULAR_EXPRESSIONS)
	{
		regex reAdrInStr(i);
		bResult = bResult || regex_search(STR_SENTENSE, reAdrInStr);
	}
	return bResult;
}

vector<bool> CProcessUrl::ReturnStateVector()  noexcept
{
	vector<bool> vec_bResult =
	{
		m_bIsFinishedProcess,
		m_bIsInitInputFileName,
		m_bIsInitOutputFileName,
		m_bIsOpenInputFile,
		m_bIsOpenOutputFile,
		m_bIsStreamInterrupted
	};
	return vec_bResult;
}

bool CProcessUrl::_IsValidNames()  noexcept
{
	if (!m_bIsInitInputFileName || !m_bIsInitOutputFileName)
		return _FAIL;

	m_fileInput.open(m_strInputFileName);

	if (!m_fileInput.is_open())
		return _FAIL;

	m_fileOutput.open(m_strOutputFileName);

	if (!m_fileOutput.is_open())
		return _FAIL;

	m_bIsOpenInputFile = true;
	m_bIsOpenOutputFile = true;

	return _SUCCESS;
}

void CProcessUrl::_SetStreamError() noexcept
{
	if (!m_fileInput.is_open())
		m_bIsOpenInputFile = false;
	if (!m_fileOutput.is_open())
		m_bIsOpenOutputFile = false;
	m_bIsStreamInterrupted = true;
	return;
}

bool CProcessUrl::_CheckInputStream() noexcept
{
	m_bIsOpenInputFile = m_fileInput.is_open();
	return m_bIsOpenInputFile;
}

bool CProcessUrl::_CheckOutputStream() noexcept
{
	m_bIsOpenOutputFile = m_fileOutput.is_open();
	return m_bIsOpenOutputFile;
}

void CProcessUrl::_ReadText() noexcept
{
	if (!_CheckInputStream())
		_SetStreamError();
	else
	{
		string strTemp;
		while (getline(m_fileInput, strTemp))
			strText.append(strTemp);
	}
	if (m_fileInput.eofbit == _SUCCESS)
		m_bIsTextRead = _SUCCESS;
	else
		_SetStreamError(); 
	m_fileInput.close();
	m_bIsOpenInputFile = _CLOSED;
	return;
}

string CProcessUrl::_CutFirstSent(const int& SENT_END) noexcept
{
	string temp = strText.substr(_BEGIN_STR, SENT_END);
	strText = strText.substr(SENT_END, strText.size() - SENT_END + _CUT_STEP);
	return temp;
}

int CProcessUrl::_FindEndSent() noexcept
{
	int nSentLength = strText.size();
	for (string i : _ENDS_OF_SENT)
	{
		int nTempPos = strText.find(i);
		if (nTempPos < nSentLength && nTempPos > _BEGIN_STR)
			nSentLength = nTempPos;
	}
	if (nSentLength == strText.size())
		return nSentLength;
	return nSentLength + _END_SENT_STEP;
}

void CProcessUrl::_ProcessText() noexcept
{
	_ASSERT(!m_bIsStreamInterrupted);
	int nPos = _FindEndSent();
	_ASSERT(nPos > _MIN_POS);
	while (nPos > _END_SENT_STEP)
	{
		string strSent = _CutFirstSent(_FindEndSent());
		if (_IsThereUrlInStr(strSent))
			m_vecResult.push_back(strSent);
		nPos = _FindEndSent();
		_ASSERT(nPos >= _MIN_POS);
	}
	m_bIsTextProcessed = _SUCCESS;
	return;
}

void CProcessUrl::_WriteText() noexcept
{
	if (!_CheckOutputStream())
		_SetStreamError();
	else
		for (string i : m_vecResult)
		{
			m_fileOutput << i << _ENTER_CHAR;
			if (!_CheckOutputStream())
			{
				_SetStreamError();
				return;
			}
		}
	m_bIsTextWritten = _SUCCESS;
	m_fileOutput.close();
	m_bIsOpenOutputFile = _CLOSED;
	return;
}

void CProcessUrl::_SetSuccessState() noexcept
{
	m_bIsFinishedProcess = true;
	return;
}

bool CProcessUrl::StartOfProcessing() noexcept
{
	if (!_IsValidNames())
		return _FAIL;

	if (m_bIsOpenInputFile)
		_ReadText();

	if (m_bIsTextRead)
		_ProcessText();

	if (m_bIsTextProcessed)
		_WriteText();

	_SetSuccessState();
	return _SUCCESS;
}