#pragma once
#include "Res/resource.h"
#include "afxcmn.h"


// ScoreDialog �Ի���

class ScoreDialog : public CDialogEx
{
	DECLARE_DYNAMIC(ScoreDialog)

public:
	ScoreDialog(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~ScoreDialog();

    virtual BOOL OnInitDialog() override;
	enum { IDD = IDD_SCOREDIALOG };

    void UpdateScore(const ::std::vector<int> &scores, const ::std::vector<int> &elapsed_times);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
    CListCtrl List;
    ::std::vector<int> Scores;
    ::std::vector<int> ElapsedTimes;

    static void ShowScoreDialog(const ::std::vector<int> &scores, const ::std::vector<int> &elapsed_times);
    static void AnalysisAndShowScoreDialog(const ::std::string &data);
};
