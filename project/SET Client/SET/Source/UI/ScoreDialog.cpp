// ScoreDialog.cpp : 实现文件
//

#include "Include/stdafx.h"
#include "SET.h"
#include "Include/UI/ScoreDialog.h"
#include "afxdialogex.h"

// ScoreDialog 对话框

IMPLEMENT_DYNAMIC(ScoreDialog, CDialogEx)

ScoreDialog::ScoreDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(ScoreDialog::IDD, pParent)
{
}

ScoreDialog::~ScoreDialog()
{
}

void ScoreDialog::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_LIST, List);
}

BEGIN_MESSAGE_MAP(ScoreDialog, CDialogEx)
END_MESSAGE_MAP()

void ScoreDialog::UpdateScore(const ::std::vector<int> &scores, const ::std::vector<int> &elapsed_times)
{
    List.DeleteAllItems();
    size_t size = scores.size();
    for (int i = 0; i < static_cast<int>(size); ++i)
    {
        ::std::wostringstream woss;
        woss << i;
        List.InsertItem(i, woss.str().c_str());
        woss = ::std::wostringstream();
        woss << scores[i];
        List.SetItemText(i, 1, woss.str().c_str());
        woss = ::std::wostringstream();
        CTimeSpan elapsed(elapsed_times[i]);
        List.SetItemText(i, 2, elapsed.Format(TEXT("%M:%S")));
        woss = ::std::wostringstream();
        woss << (elapsed_times[i] != 0 ? static_cast<double>(scores[i]) / elapsed_times[i] * 60.0 : 0);
        List.SetItemText(i, 3, woss.str().c_str());
    }

    List.SetColumnWidth(0, LVSCW_AUTOSIZE_USEHEADER);
    List.SetColumnWidth(1, LVSCW_AUTOSIZE_USEHEADER);
    List.SetColumnWidth(2, LVSCW_AUTOSIZE_USEHEADER);
    List.SetColumnWidth(3, LVSCW_AUTOSIZE_USEHEADER);
    InvalidateRect(NULL);
}

void ScoreDialog::ShowScoreDialog(const ::std::vector<int> &scores, const ::std::vector<int> &elapsed_times)
{
    ScoreDialog dialog(AfxGetMainWnd());
    //dialog.UpdateData();
    dialog.Scores = scores;
    dialog.ElapsedTimes = elapsed_times;
    dialog.DoModal();
    return;
}

void ScoreDialog::AnalysisAndShowScoreDialog(const ::std::string &data)
{
    ::std::vector<int> scores, elapsed_times;
    ::std::istringstream iss(data);
    int score, elapsed_time;
    while (!iss.eof())
    {
        iss >> score >> elapsed_time;
        if (iss)
        {
            scores.push_back(score);
            elapsed_times.push_back(elapsed_time);
        }
    }
    ShowScoreDialog(scores, elapsed_times);

}

BOOL ScoreDialog::OnInitDialog()
{
    BOOL retval = __super::OnInitDialog();
    UpdateData();
    List.InsertColumn(0, TEXT("ID"));
    List.InsertColumn(1, TEXT("Score"));
    List.InsertColumn(2, TEXT("Elapsed Time"));
    List.InsertColumn(3, TEXT("Score Per Minute"));

    UpdateScore(Scores, ElapsedTimes);
    return retval;
}

// ScoreDialog 消息处理程序
