#pragma once
#include "Res/resource.h"
#include "afxcmn.h"


// ScoreDialog 对话框

class ScoreDialog : public CDialogEx
{
	DECLARE_DYNAMIC(ScoreDialog)

public:
	ScoreDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~ScoreDialog();

    virtual BOOL OnInitDialog() override;
	enum { IDD = IDD_SCOREDIALOG };

    void UpdateScore(const ::std::vector<int> &scores, const ::std::vector<int> &elapsed_times);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
    CListCtrl List;
    ::std::vector<int> Scores;
    ::std::vector<int> ElapsedTimes;

    static void ShowScoreDialog(const ::std::vector<int> &scores, const ::std::vector<int> &elapsed_times);
    static void AnalysisAndShowScoreDialog(const ::std::string &data);
};
