VERSION 5.00
Begin VB.Form formMain 
   Caption         =   "Automatic Paramters Optimizer"
   ClientHeight    =   6255
   ClientLeft      =   60
   ClientTop       =   345
   ClientWidth     =   12495
   LinkTopic       =   "Form1"
   ScaleHeight     =   6255
   ScaleWidth      =   12495
   StartUpPosition =   3  'Windows 기본값
   Begin VB.CheckBox chk_best_fitness 
      Caption         =   "best_fitness 비교"
      Height          =   300
      Left            =   10710
      TabIndex        =   82
      Top             =   5295
      Width           =   1740
   End
   Begin VB.CommandButton Command5 
      Caption         =   "set diff"
      Height          =   315
      Left            =   8925
      TabIndex        =   81
      Top             =   5205
      Width           =   1275
   End
   Begin VB.TextBox txt_diff_spin_count 
      Height          =   270
      Left            =   8070
      TabIndex        =   79
      Text            =   "200"
      Top             =   5565
      Width           =   675
   End
   Begin VB.TextBox txt_diff_local_opt_count 
      Height          =   270
      Left            =   8085
      TabIndex        =   77
      Text            =   "1000"
      Top             =   5205
      Width           =   675
   End
   Begin VB.TextBox txt_diff_p 
      Height          =   270
      Left            =   5400
      TabIndex        =   75
      Text            =   "0.05"
      Top             =   5595
      Width           =   675
   End
   Begin VB.TextBox txt_diff_pop 
      Height          =   270
      Left            =   5415
      TabIndex        =   73
      Text            =   "10"
      Top             =   5265
      Width           =   675
   End
   Begin VB.TextBox Text2 
      Height          =   1005
      Left            =   165
      Locked          =   -1  'True
      MultiLine       =   -1  'True
      ScrollBars      =   2  '수직
      TabIndex        =   72
      Text            =   "formMain.frx":0000
      Top             =   5130
      Width           =   3660
   End
   Begin VB.CheckBox Check2 
      Caption         =   "Change opt cond.."
      Height          =   210
      Left            =   4275
      TabIndex        =   71
      Top             =   4755
      Width           =   2250
   End
   Begin VB.CommandButton Command4 
      Caption         =   "set opt index"
      Height          =   315
      Left            =   2505
      TabIndex        =   70
      Top             =   4695
      Width           =   1275
   End
   Begin VB.TextBox Text1 
      Height          =   270
      Left            =   1860
      TabIndex        =   69
      Top             =   4710
      Width           =   540
   End
   Begin VB.CheckBox Check1 
      Caption         =   "Auto optimize"
      Height          =   285
      Left            =   60
      TabIndex        =   68
      Top             =   4695
      Width           =   1680
   End
   Begin VB.TextBox txt_best_time 
      Height          =   330
      Left            =   9150
      TabIndex        =   66
      Top             =   4185
      Width           =   1350
   End
   Begin VB.TextBox txt_avg_time 
      Height          =   330
      Left            =   6765
      TabIndex        =   64
      Top             =   4185
      Width           =   1440
   End
   Begin VB.TextBox txt_best_fitness 
      Height          =   330
      Left            =   3975
      TabIndex        =   62
      Top             =   4200
      Width           =   1800
   End
   Begin VB.TextBox txt_avg_fitness 
      Height          =   330
      Left            =   1065
      TabIndex        =   60
      Top             =   4185
      Width           =   1800
   End
   Begin VB.Timer Timer1 
      Enabled         =   0   'False
      Interval        =   5000
      Left            =   10350
      Top             =   4020
   End
   Begin VB.CommandButton Command1 
      Caption         =   "Start"
      Height          =   375
      Left            =   10950
      TabIndex        =   55
      Top             =   5730
      Width           =   1455
   End
   Begin VB.Frame Frame4 
      Caption         =   "Mutation"
      Height          =   1485
      Left            =   45
      TabIndex        =   31
      Top             =   2505
      Width           =   6150
      Begin VB.CheckBox chk_force_mutation2 
         Caption         =   "mutation2 강제적용"
         Height          =   315
         Left            =   4155
         TabIndex        =   48
         Top             =   1095
         Width           =   1890
      End
      Begin VB.TextBox txt_do_mutation2_p 
         Height          =   270
         Left            =   4980
         TabIndex        =   45
         Top             =   255
         Width           =   1035
      End
      Begin VB.TextBox txt_mutation2_p 
         Height          =   270
         Left            =   4980
         TabIndex        =   44
         Top             =   675
         Width           =   1050
      End
      Begin VB.TextBox txt_do_mutation1_p 
         Height          =   270
         Left            =   1965
         TabIndex        =   41
         Top             =   255
         Width           =   1035
      End
      Begin VB.TextBox txt_mutation1_p 
         Height          =   270
         Left            =   1965
         TabIndex        =   40
         Top             =   675
         Width           =   1050
      End
      Begin VB.Label Label26 
         Caption         =   "mutation2 시행확률"
         Height          =   255
         Left            =   3150
         TabIndex        =   47
         Top             =   300
         Width           =   1785
      End
      Begin VB.Label Label24 
         Caption         =   "mutation2 변이율"
         Height          =   255
         Left            =   3150
         TabIndex        =   46
         Top             =   705
         Width           =   1830
      End
      Begin VB.Label Label21 
         Caption         =   "mutation1 시행확률"
         Height          =   255
         Left            =   135
         TabIndex        =   43
         Top             =   300
         Width           =   1785
      End
      Begin VB.Label Label20 
         Caption         =   "mutation1 변이율"
         Height          =   255
         Left            =   135
         TabIndex        =   42
         Top             =   705
         Width           =   1830
      End
   End
   Begin VB.Frame Frame3 
      Caption         =   "최적화"
      Height          =   1950
      Left            =   6270
      TabIndex        =   24
      Top             =   2055
      Width           =   6150
      Begin VB.TextBox txt_do_spin_p 
         Height          =   270
         Left            =   5010
         TabIndex        =   36
         Top             =   285
         Width           =   1035
      End
      Begin VB.TextBox txt_spin_count 
         Height          =   270
         Left            =   5010
         TabIndex        =   35
         Top             =   705
         Width           =   1050
      End
      Begin VB.TextBox txt_do_spin_apply_p 
         Height          =   270
         Left            =   4995
         TabIndex        =   34
         Top             =   1095
         Width           =   1050
      End
      Begin VB.TextBox txt_do_deep_worse_optimize_p 
         Height          =   270
         Left            =   1980
         TabIndex        =   33
         Top             =   1470
         Width           =   1050
      End
      Begin VB.TextBox txt_do_optimize_p 
         Height          =   270
         Left            =   1995
         TabIndex        =   27
         Top             =   285
         Width           =   1035
      End
      Begin VB.TextBox txt_local_optimize_count 
         Height          =   270
         Left            =   1995
         TabIndex        =   26
         Top             =   705
         Width           =   1050
      End
      Begin VB.TextBox txt_do_deep_better_optimize_p 
         Height          =   270
         Left            =   1980
         TabIndex        =   25
         Top             =   1095
         Width           =   1050
      End
      Begin VB.Label Label25 
         Caption         =   "spin 최적화 시행확률"
         Height          =   255
         Left            =   3180
         TabIndex        =   39
         Top             =   330
         Width           =   1785
      End
      Begin VB.Label Label17 
         Caption         =   "spin 최적화 시행회수"
         Height          =   255
         Left            =   3180
         TabIndex        =   38
         Top             =   735
         Width           =   1830
      End
      Begin VB.Label Label16 
         Caption         =   "spin 적용 확률"
         Height          =   255
         Left            =   3165
         TabIndex        =   37
         Top             =   1140
         Width           =   1830
      End
      Begin VB.Label Label13 
         Caption         =   "deeper worse 확률"
         Height          =   255
         Left            =   150
         TabIndex        =   32
         Top             =   1515
         Width           =   1830
      End
      Begin VB.Label Label18 
         Caption         =   "2opt 최적화 시행확률"
         Height          =   255
         Left            =   165
         TabIndex        =   30
         Top             =   330
         Width           =   1785
      End
      Begin VB.Label Label15 
         Caption         =   "2opt 최적화 시행회수"
         Height          =   255
         Left            =   165
         TabIndex        =   29
         Top             =   735
         Width           =   1830
      End
      Begin VB.Label Label14 
         Caption         =   "deeper better 확률"
         Height          =   255
         Left            =   150
         TabIndex        =   28
         Top             =   1140
         Width           =   1830
      End
   End
   Begin VB.Frame Frame2 
      Caption         =   "Selection && Crossover"
      Height          =   1905
      Left            =   6270
      TabIndex        =   11
      Top             =   75
      Width           =   6165
      Begin VB.TextBox txt_xover_uniform_p 
         Height          =   270
         Left            =   5175
         TabIndex        =   23
         Top             =   1485
         Width           =   885
      End
      Begin VB.TextBox txt_change_xover_src_p 
         Height          =   270
         Left            =   3240
         TabIndex        =   21
         Top             =   1485
         Width           =   885
      End
      Begin VB.TextBox txt_xover_npt 
         Height          =   270
         Left            =   4965
         TabIndex        =   19
         Top             =   1125
         Width           =   375
      End
      Begin VB.TextBox txt_do_xover_p 
         Height          =   270
         Left            =   1890
         TabIndex        =   17
         Top             =   780
         Width           =   780
      End
      Begin VB.ComboBox cmb_xover_kind 
         Height          =   300
         ItemData        =   "formMain.frx":01A1
         Left            =   1890
         List            =   "formMain.frx":01B4
         Style           =   2  '드롭다운 목록
         TabIndex        =   14
         Top             =   1110
         Width           =   2655
      End
      Begin VB.TextBox txt_tournament_winner_p 
         Height          =   270
         Left            =   3195
         TabIndex        =   13
         Top             =   300
         Width           =   990
      End
      Begin VB.Label Label12 
         Caption         =   "Uniform p"
         Height          =   255
         Left            =   4290
         TabIndex        =   22
         Top             =   1530
         Width           =   885
      End
      Begin VB.Label Label11 
         Caption         =   "확률N점변화비율"
         Height          =   255
         Left            =   1815
         TabIndex        =   20
         Top             =   1530
         Width           =   1605
      End
      Begin VB.Label Label10 
         Caption         =   "점 교배"
         Height          =   255
         Left            =   5340
         TabIndex        =   18
         Top             =   1155
         Width           =   645
      End
      Begin VB.Label Label9 
         Caption         =   "Crossover 시행확률"
         Height          =   255
         Left            =   150
         TabIndex        =   16
         Top             =   825
         Width           =   1755
      End
      Begin VB.Label Label8 
         Caption         =   "Crossover 종류"
         Height          =   270
         Left            =   135
         TabIndex        =   15
         Top             =   1155
         Width           =   1590
      End
      Begin VB.Label Label7 
         Caption         =   "Tournament selection 승자 선택률"
         Height          =   270
         Left            =   120
         TabIndex        =   12
         Top             =   345
         Width           =   2910
      End
   End
   Begin VB.Frame Frame1 
      Caption         =   "일반사항 && replacement && 종료조건"
      Height          =   2310
      Left            =   45
      TabIndex        =   0
      Top             =   90
      Width           =   6150
      Begin VB.CommandButton Command3 
         Caption         =   "save"
         Height          =   285
         Left            =   5475
         TabIndex        =   67
         Top             =   300
         Width           =   600
      End
      Begin VB.CommandButton Command2 
         Caption         =   "load"
         Height          =   285
         Left            =   4800
         TabIndex        =   58
         Top             =   300
         Width           =   600
      End
      Begin VB.TextBox txt_test_count 
         Height          =   270
         Left            =   4845
         TabIndex        =   57
         Top             =   1290
         Width           =   1050
      End
      Begin VB.TextBox txt_converge_count 
         Height          =   270
         Left            =   5040
         TabIndex        =   53
         Top             =   1890
         Width           =   1035
      End
      Begin VB.TextBox txt_converge_rate_p 
         Height          =   270
         Left            =   2025
         TabIndex        =   51
         Top             =   1890
         Width           =   1035
      End
      Begin VB.TextBox txt_replace_bad_p 
         Height          =   270
         Left            =   1740
         TabIndex        =   49
         Top             =   1320
         Width           =   1035
      End
      Begin VB.TextBox txt_generation_size 
         Height          =   270
         Left            =   4455
         TabIndex        =   10
         Top             =   735
         Width           =   1050
      End
      Begin VB.TextBox txt_population_size 
         Height          =   270
         Left            =   1620
         TabIndex        =   8
         Top             =   735
         Width           =   1050
      End
      Begin VB.TextBox txt_max_width 
         Height          =   270
         Left            =   4350
         TabIndex        =   6
         Top             =   300
         Width           =   420
      End
      Begin VB.TextBox txt_min_width 
         Height          =   270
         Left            =   3465
         TabIndex        =   4
         Top             =   285
         Width           =   540
      End
      Begin VB.TextBox txt_outputfile 
         Height          =   270
         Left            =   1335
         TabIndex        =   2
         Top             =   300
         Width           =   780
      End
      Begin VB.Label Label23 
         Caption         =   "테스트 시행회수"
         Height          =   255
         Left            =   3435
         TabIndex        =   56
         Top             =   1350
         Width           =   1455
      End
      Begin VB.Label Label22 
         Caption         =   "종료 converge count"
         Height          =   255
         Left            =   3210
         TabIndex        =   54
         Top             =   1935
         Width           =   1785
      End
      Begin VB.Label Label6 
         Caption         =   "종료 Converge rate"
         Height          =   255
         Left            =   195
         TabIndex        =   52
         Top             =   1935
         Width           =   1785
      End
      Begin VB.Label Label19 
         Caption         =   "replace bad 확률"
         Height          =   255
         Left            =   270
         TabIndex        =   50
         Top             =   1365
         Width           =   1515
      End
      Begin VB.Label Label5 
         Caption         =   "Generation size"
         Height          =   255
         Left            =   3045
         TabIndex        =   9
         Top             =   795
         Width           =   1455
      End
      Begin VB.Label Label4 
         Caption         =   "Population size"
         Height          =   255
         Left            =   255
         TabIndex        =   7
         Top             =   795
         Width           =   1305
      End
      Begin VB.Label Label3 
         Caption         =   "M="
         Height          =   255
         Left            =   4050
         TabIndex        =   5
         Top             =   345
         Width           =   570
      End
      Begin VB.Label Label2 
         Caption         =   "문제정의   m="
         Height          =   255
         Left            =   2280
         TabIndex        =   3
         Top             =   330
         Width           =   1530
      End
      Begin VB.Label Label1 
         Caption         =   "Output file #"
         Height          =   255
         Left            =   255
         TabIndex        =   1
         Top             =   360
         Width           =   1095
      End
   End
   Begin VB.Label Label34 
      Caption         =   "diff spin count"
      Height          =   330
      Left            =   6180
      TabIndex        =   80
      Top             =   5625
      Width           =   1695
   End
   Begin VB.Label Label33 
      Caption         =   "diff local opt count"
      Height          =   330
      Left            =   6195
      TabIndex        =   78
      Top             =   5265
      Width           =   1695
   End
   Begin VB.Label Label32 
      Caption         =   "diff p"
      Height          =   330
      Left            =   4260
      TabIndex        =   76
      Top             =   5655
      Width           =   1350
   End
   Begin VB.Label Label31 
      Caption         =   "diff pop,gen"
      Height          =   330
      Left            =   4275
      TabIndex        =   74
      Top             =   5325
      Width           =   1350
   End
   Begin VB.Label Label30 
      Caption         =   "best_time"
      Height          =   300
      Left            =   8235
      TabIndex        =   65
      Top             =   4260
      Width           =   1005
   End
   Begin VB.Label Label29 
      Caption         =   "avg_time"
      Height          =   300
      Left            =   5880
      TabIndex        =   63
      Top             =   4245
      Width           =   1005
   End
   Begin VB.Label Label28 
      Caption         =   "best_fitness"
      Height          =   300
      Left            =   2970
      TabIndex        =   61
      Top             =   4260
      Width           =   1005
   End
   Begin VB.Label Label27 
      Caption         =   "avg_fitness"
      Height          =   300
      Left            =   60
      TabIndex        =   59
      Top             =   4245
      Width           =   1005
   End
End
Attribute VB_Name = "formMain"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Dim g_section As String

Dim cmd As String

Dim g_test_count As Integer
Dim g_index As Integer
Dim g_diff As Double

Dim avg_fitness As Double
Dim best_fitness, avg_time, shortest_time As Double
    
Dim g_diff_local_opt_count As Integer
Dim g_diff_p As Double
Dim g_diff_pop As Integer
Dim g_diff_spin_count As Integer

Private Sub Command1_Click()
    
    ExecuteCommand
    
End Sub

Private Sub Command2_Click()

    LoadParams

End Sub

Private Sub ExecuteCommand()
    
 '   On Error Resume Next
 
    If Dir("c:\done.ga") = "done.ga" Then
        Kill "c:\done.ga"
    End If
    
    If Dir("c:\done.msg") = "done.msg" Then
        Kill "c:\done.msg"
    End If
    
    DoEvents
    
    cmd = ".\gahw2.exe" & _
    " " & ".\\" & txt_outputfile.Text & ".txt" & _
    " " & ".\\" & txt_outputfile.Text & ".xls" & _
    " " & txt_min_width.Text & _
    " " & txt_max_width.Text & _
    " " & txt_population_size.Text & _
    " " & txt_generation_size.Text & _
    " " & txt_tournament_winner_p.Text & _
    " " & txt_xover_uniform_p.Text & _
    " " & txt_replace_bad_p.Text & _
    " " & txt_do_optimize_p.Text & _
    " " & txt_local_optimize_count.Text & _
    " " & txt_do_deep_better_optimize_p.Text & _
    " " & txt_do_deep_worse_optimize_p.Text & _
    " " & txt_do_spin_p.Text & _
    " " & txt_spin_count.Text & _
    " " & txt_do_spin_apply_p.Text & _
    " " & txt_do_mutation1_p.Text & _
    " " & txt_mutation1_p.Text & _
    " " & txt_do_mutation2_p.Text & _
    " " & txt_mutation2_p.Text & _
    " " & chk_force_mutation2.Value
    
    cmd = cmd & _
    " " & txt_converge_rate_p.Text & _
    " " & txt_converge_count.Text & _
    " " & txt_test_count.Text & _
    " " & cmb_xover_kind.ListIndex & _
    " " & txt_xover_npt.Text & _
    " " & txt_change_xover_src_p.Text & _
    " " & txt_do_xover_p.Text

    Call Shell(cmd, vbMinimizedNoFocus)
    
    DoEvents
    
    Timer1.Enabled = True
    
End Sub

Private Sub SaveParams()
    g_section = txt_min_width & "_" & txt_max_width
    
    Call SaveSetting("GAHW2", g_section, "g_population_size", txt_population_size.Text)
    Call SaveSetting("GAHW2", g_section, "g_generation_size", txt_generation_size.Text)
    Call SaveSetting("GAHW2", g_section, "g_tournament_winner_p", txt_tournament_winner_p.Text)
    Call SaveSetting("GAHW2", g_section, "g_xover_uniform_p", txt_xover_uniform_p.Text)
    Call SaveSetting("GAHW2", g_section, "g_replace_bad_p", txt_replace_bad_p.Text)
    
    Call SaveSetting("GAHW2", g_section, "g_do_optimize_p", txt_do_optimize_p.Text)
    Call SaveSetting("GAHW2", g_section, "g_local_optimize_count", txt_local_optimize_count.Text)
    Call SaveSetting("GAHW2", g_section, "g_do_deep_better_optimize_p", txt_do_deep_better_optimize_p.Text)
    Call SaveSetting("GAHW2", g_section, "g_do_deep_worse_optimize_p", txt_do_deep_worse_optimize_p.Text)
    Call SaveSetting("GAHW2", g_section, "g_do_spin_p", txt_do_spin_p.Text)
    
    Call SaveSetting("GAHW2", g_section, "g_spin_count", txt_spin_count.Text)
    Call SaveSetting("GAHW2", g_section, "g_do_spin_apply_p", txt_do_spin_apply_p.Text)
    
    Call SaveSetting("GAHW2", g_section, "g_do_mutation1_p", txt_do_mutation1_p.Text)
    Call SaveSetting("GAHW2", g_section, "g_mutation1_p", txt_mutation1_p.Text)
    Call SaveSetting("GAHW2", g_section, "g_do_mutation2_p", txt_do_mutation2_p.Text)
    Call SaveSetting("GAHW2", g_section, "g_mutation2_p", txt_mutation2_p.Text)
    Call SaveSetting("GAHW2", g_section, "g_force_mutation2", chk_force_mutation2.Value)
    
    Call SaveSetting("GAHW2", g_section, "g_converge_rate_p", txt_converge_rate_p.Text)
    Call SaveSetting("GAHW2", g_section, "g_converge_count", txt_converge_count.Text)
    Call SaveSetting("GAHW2", g_section, "g_test_count", txt_test_count.Text)
    
    Call SaveSetting("GAHW2", g_section, "g_xover_kind", Str(cmb_xover_kind.ListIndex))
    Call SaveSetting("GAHW2", g_section, "g_xover_npt", txt_xover_npt.Text)
    Call SaveSetting("GAHW2", g_section, "g_change_xover_src_p", txt_change_xover_src_p.Text)
    Call SaveSetting("GAHW2", g_section, "g_do_xover_p", txt_do_xover_p.Text)

    Call SaveSetting("GAHW2", g_section, "avg_fitness", txt_avg_fitness)
    Call SaveSetting("GAHW2", g_section, "best_fitness", txt_best_fitness)
    Call SaveSetting("GAHW2", g_section, "avg_time", txt_avg_time)
    Call SaveSetting("GAHW2", g_section, "best_time", txt_best_time)

End Sub

Private Sub LoadParams()
    
    Timer1.Enabled = False
    
    g_section = txt_min_width & "_" & txt_max_width

    txt_population_size.Text = GetSetting("GAHW2", g_section, "g_population_size", "130")
    txt_generation_size.Text = GetSetting("GAHW2", g_section, "g_generation_size", "50")
    txt_tournament_winner_p.Text = GetSetting("GAHW2", g_section, "g_tournament_winner_p", "1")
    txt_xover_uniform_p.Text = GetSetting("GAHW2", g_section, "g_xover_uniform_p", "0.697")
    txt_replace_bad_p.Text = GetSetting("GAHW2", g_section, "g_replace_bad_p", "0.195")
    
    txt_do_optimize_p.Text = GetSetting("GAHW2", g_section, "g_do_optimize_p", "1")
    txt_local_optimize_count.Text = GetSetting("GAHW2", g_section, "g_local_optimize_count", "6000")
    txt_do_deep_better_optimize_p.Text = GetSetting("GAHW2", g_section, "g_do_deep_better_optimize_p", "1")
    txt_do_deep_worse_optimize_p.Text = GetSetting("GAHW2", g_section, "g_do_deep_worse_optimize_p", "0")
    txt_do_spin_p.Text = GetSetting("GAHW2", g_section, "g_do_spin_p", "0")
    
    txt_spin_count.Text = GetSetting("GAHW2", g_section, "g_spin_count", "1000")
    txt_do_spin_apply_p.Text = GetSetting("GAHW2", g_section, "g_do_spin_apply_p", "1")
    
    txt_do_mutation1_p.Text = GetSetting("GAHW2", g_section, "g_do_mutation1_p", "0.1")
    txt_mutation1_p.Text = GetSetting("GAHW2", g_section, "g_mutation1_p", "0.01")
    txt_do_mutation2_p.Text = GetSetting("GAHW2", g_section, "g_do_mutation2_p", "0.1")
    txt_mutation2_p.Text = GetSetting("GAHW2", g_section, "g_mutation2_p", "0.01")
    chk_force_mutation2.Value = GetSetting("GAHW2", g_section, "g_force_mutation2", "0")
    
    txt_converge_rate_p.Text = GetSetting("GAHW2", g_section, "g_converge_rate_p", "0.99")
    txt_converge_count.Text = GetSetting("GAHW2", g_section, "g_converge_count", "1")
    txt_test_count.Text = GetSetting("GAHW2", g_section, "g_test_count", "10")
    
    cmb_xover_kind.ListIndex = Val(GetSetting("GAHW2", g_section, "g_xover_kind", "4"))
    txt_xover_npt.Text = GetSetting("GAHW2", g_section, "g_xover_npt", "2")
    txt_change_xover_src_p.Text = GetSetting("GAHW2", g_section, "g_change_xover_src_p", "0.16")
    txt_do_xover_p.Text = GetSetting("GAHW2", g_section, "g_do_xover_p", "1")

    txt_avg_fitness = GetSetting("GAHW2", g_section, "avg_fitness", "999999")
    txt_best_fitness = GetSetting("GAHW2", g_section, "best_fitness", "999999")
    txt_avg_time = GetSetting("GAHW2", g_section, "avg_time", "99999")
    txt_best_time = GetSetting("GAHW2", g_section, "best_time", "99999")
    
End Sub

Private Sub Command3_Click()
    SaveParams
    
End Sub

Private Sub Command4_Click()
    g_index = Val(Text1.Text)
End Sub

Private Sub Command5_Click()
Call SaveSetting("GAHW2", "optimizer", "g_diff_local_opt_count", txt_diff_local_opt_count.Text)
Call GetSetting("GAHW2", "optimizer", "g_diff_p", txt_diff_p.Text)
Call GetSetting("GAHW2", "optimizer", "g_diff_pop", txt_diff_pop.Text)
Call GetSetting("GAHW2", "optimizer", "g_diff_spin_count", txt_diff_spin_count.Text)

g_diff_local_opt_count = GetSetting("GAHW2", "optimizer", "g_diff_local_opt_count", "1000")
g_diff_p = GetSetting("GAHW2", "optimizer", "g_diff_p", "0.05")
g_diff_pop = GetSetting("GAHW2", "optimizer", "g_diff_pop", "10")
g_diff_spin_count = GetSetting("GAHW2", "optimizer", "g_diff_spin_count", "200")

End Sub

Private Sub Form_Load()

    txt_outputfile.Text = GetSetting("GAHW2", "optimizer", "g_outputfile", "1")
    txt_min_width.Text = GetSetting("GAHW2", "optimizer", "g_min_width", "3")
    txt_max_width.Text = GetSetting("GAHW2", "optimizer", "g_max_width", "5")
            
    g_test_count = 0
    g_index = 0
    g_diff = 1

    Me.Show

g_diff_local_opt_count = GetSetting("GAHW2", "optimizer", "g_diff_local_opt_count", "1000")
g_diff_p = GetSetting("GAHW2", "optimizer", "g_diff_p", "0.05")
g_diff_pop = GetSetting("GAHW2", "optimizer", "g_diff_pop", "10")
g_diff_spin_count = GetSetting("GAHW2", "optimizer", "g_diff_spin_count", "200")

txt_diff_local_opt_count.Text = Val(g_diff_local_opt_count)
txt_diff_p.Text = Val(g_diff_p)
txt_diff_pop.Text = Val(g_diff_pop)
txt_diff_spin_count.Text = Val(g_diff_spin_count)

End Sub

Private Sub Form_Unload(Cancel As Integer)
    Call GetSetting("GAHW2", "optimizer", "g_min_width", txt_min_width.Text)
    Call GetSetting("GAHW2", "optimizer", "g_max_width", txt_max_width.Text)

End Sub

Private Sub Timer1_Timer()
    
    If Dir("c:\done.msg") <> "done.msg" Then Exit Sub
    
    Timer1.Enabled = False
    DoEvents
    
    txt_outputfile.Text = Val(txt_outputfile.Text) + 1
    Call GetSetting("GAHW2", "optimizer", "g_outputfile", txt_outputfile.Text)
    

    
    Open "c:\done.ga" For Input As #1
    
    Input #1, avg_fitness, best_fitness, avg_time, shortest_time
        Debug.Print avg_fitness, best_fitness, avg_time, shortest_time
    DoEvents
    Close #1
    
    If Dir("c:\params" & g_section & ".txt") = "" Then
        Open "c:\params" & g_section & ".txt" For Output As #2
    Else
        Open "c:\params" & g_section & ".txt" For Append As #2
    End If
    
    Print #2, avg_fitness & vbTab & best_fitness & vbTab & avg_time & vbTab & shortest_time & vbTab & cmd
    
    Close #2
    
    If Check1.Value = 1 Then
    
    ''''''''''''''''''''''' 알고리즘
    ClearFontBold
    optimizeParams
        
    Else
        
        If Val(txt_avg_fitness.Text) > avg_fitness Then
        
            txt_avg_fitness.Text = avg_fitness
            txt_best_fitness.Text = best_fitness
            txt_avg_time.Text = avg_time
            txt_best_time.Text = shortest_time
            
        End If
    
    End If
    
    
End Sub

Private Sub optimizeParams()
    
    If (Val(txt_avg_fitness.Text) > avg_fitness) Or ((Val(txt_avg_fitness.Text) = avg_fitness) And (Val(txt_avg_time.Text) > avg_time)) Or (chk_best_fitness.Value = 1 And Val(txt_best_fitness.Text) > best_fitness) Then
        
        txt_avg_fitness.Text = avg_fitness
        txt_best_fitness.Text = best_fitness
        txt_avg_time.Text = avg_time
        txt_best_time.Text = shortest_time
        
        SaveParams
        
        Do While (GoNextOptimize(1) = False)
        Loop
        
        ExecuteCommand
                
    Else
        
        LoadParams
        
        Do While (GoNextOptimize(0) = False)
        Loop
        
        ExecuteCommand
        
    End If

End Sub
Private Function GoNextOptimize(index As Integer) As Boolean
    Dim t As String
    Dim ret As Boolean
    
    ret = True
    
    If index = 0 Then
        g_diff = -g_diff
        g_test_count = g_test_count + 1
    End If
    
    If g_test_count > 1 Then
        If Check2.Value = 1 Then
         g_index = (g_index + 1) Mod 20
        End If
         g_test_count = 0
     End If
            
    Select Case g_index
        Case 0  'population size
            g_diff = ((g_diff) / Abs(g_diff)) * g_diff_pop
            txt_population_size.Text = Val(txt_population_size.Text) + g_diff
            
            If Val(txt_population_size.Text) < 10 Then
                txt_population_size.Text = 10
            End If
            
            txt_population_size.FontBold = True
            
        Case 1  'generation size
            g_diff = ((g_diff) / Abs(g_diff)) * g_diff_pop
            txt_generation_size.Text = Val(txt_generation_size.Text) + g_diff
            
            If Val(txt_generation_size.Text) < 10 Then
                txt_generation_size.Text = 10
            End If
            
            If Val(txt_generation_size.Text) > Val(txt_population_size.Text) Then
                txt_generation_size.Text = txt_population_size.Text
            End If
            txt_generation_size.FontBold = True
        
        Case 2  'tournament winner p
            g_diff = ((g_diff) / Abs(g_diff)) * g_diff_p
            
            txt_tournament_winner_p.Text = Val(txt_tournament_winner_p.Text) + g_diff
            
            If Val(txt_tournament_winner_p.Text) < 0 Then
                txt_tournament_winner_p.Text = 0
            End If
            
            If Val(txt_tournament_winner_p.Text) > 1 Then
                txt_tournament_winner_p.Text = 1
            End If
            txt_tournament_winner_p.FontBold = True
        Case 3  'replace bad p
            g_diff = ((g_diff) / Abs(g_diff)) * g_diff_p

            t = txt_replace_bad_p.Text
            
            t = Val(t) + g_diff
            
            If Val(t) < 0 Then
                t = 0
            End If
            
            If Val(t) > 1 Then
                t = 1
            End If
            
            txt_replace_bad_p.Text = t
            txt_replace_bad_p.FontBold = True
            
        Case 4  'do optimize p
            g_diff = ((g_diff) / Abs(g_diff)) * g_diff_p
            t = txt_do_optimize_p.Text
            
            t = Val(t) + g_diff
            
            If Val(t) < 0 Then
                t = 0
            End If
            
            If Val(t) > 1 Then
                t = 1
            End If
            
            txt_do_optimize_p.Text = t
            txt_do_optimize_p.FontBold = True
        Case 5  'local optimize count
            g_diff = ((g_diff) / Abs(g_diff)) * g_diff_local_opt_count
            
            t = txt_local_optimize_count.Text
            
            t = Val(t) + g_diff
            
            If Val(t) < 0 Then
                t = 0
            End If
            
            txt_local_optimize_count.Text = t
            txt_local_optimize_count.FontBold = True
        Case 6  'deep better optimize
            g_diff = ((g_diff) / Abs(g_diff)) * g_diff_p
            t = txt_do_deep_better_optimize_p.Text
            
            t = Val(t) + g_diff
            
            If Val(t) < 0 Then
                t = 0
            End If
            
            If Val(t) > 1 Then
                t = 1
            End If
            
            txt_do_deep_better_optimize_p.Text = t
            txt_do_deep_better_optimize_p.FontBold = True
            
        Case 7  'deep worse optimize
            g_diff = ((g_diff) / Abs(g_diff)) * g_diff_p
            t = txt_do_deep_worse_optimize_p.Text
            
            t = Val(t) + g_diff
            
            If Val(t) < 0 Then
                t = 0
            End If
            
            If Val(t) > 1 Then
                t = 1
            End If
            
            txt_do_deep_worse_optimize_p = t
            txt_do_deep_worse_optimize_p.FontBold = True
        Case 8  'do spin p
            g_diff = ((g_diff) / Abs(g_diff)) * g_diff_p
            t = txt_do_spin_p.Text
            
            t = Val(t) + g_diff
            
            If Val(t) < 0 Then
                t = 0
            End If
            
            If Val(t) > 1 Then
                t = 1
            End If
            
            txt_do_spin_p = t
            txt_do_spin_p.FontBold = True
            
        Case 9 'spin count
            g_diff = ((g_diff) / Abs(g_diff)) * g_diff_spin_count
            
            t = txt_spin_count.Text
            
            t = Val(t) + g_diff
            
            If Val(t) < 0 Then
                t = 0
            End If
            
            txt_spin_count = t
            txt_spin_count.FontBold = True
            
        Case 10 'do spin apply p
            g_diff = ((g_diff) / Abs(g_diff)) * g_diff_p
            t = txt_do_spin_apply_p.Text
            
            t = Val(t) + g_diff
            
            If Val(t) < 0 Then
                t = 0
            End If
            
            If Val(t) > 1 Then
                t = 1
            End If
            
            txt_do_spin_apply_p = t
            txt_do_spin_apply_p.FontBold = True
            
        Case 11 'do mutation1 p
            g_diff = ((g_diff) / Abs(g_diff)) * g_diff_p
            t = txt_do_mutation1_p.Text
            
            t = Val(t) + g_diff
            
            If Val(t) < 0 Then
                t = 0
            End If
            
            If Val(t) > 1 Then
                t = 1
            End If
            
            txt_do_mutation1_p = t
            txt_do_mutation1_p.FontBold = True
        Case 12 'mutation1 p
            g_diff = ((g_diff) / Abs(g_diff)) * g_diff_p
            t = txt_mutation1_p.Text
            
            t = Val(t) + g_diff
            
            If Val(t) < 0 Then
                t = 0
            End If
            
            If Val(t) > 1 Then
                t = 1
            End If
            
            txt_mutation1_p = t
            txt_mutation1_p.FontBold = True
            
        Case 13 'do mutation2_p
            g_diff = ((g_diff) / Abs(g_diff)) * g_diff_p
            t = txt_do_mutation2_p.Text
            
            t = Val(t) + g_diff
            
            If Val(t) < 0 Then
                t = 0
            End If
            
            If Val(t) > 1 Then
                t = 1
            End If
            
            txt_do_mutation2_p = t
            txt_do_mutation2_p.FontBold = True
            
        Case 14 'mutation2_p
            g_diff = ((g_diff) / Abs(g_diff)) * g_diff_p
            t = txt_mutation2_p.Text
            
            t = Val(t) + g_diff
            
            If Val(t) < 0 Then
                t = 0
            End If
            
            If Val(t) > 1 Then
                t = 1
            End If
            
            txt_mutation2_p = t
            txt_mutation2_p.FontBold = True
            
        Case 15 'g_force_mutation2 ******
            
            chk_force_mutation2.Value = 1 - Val(chk_force_mutation2.Value)
            chk_force_mutation2.FontBold = True
            
'        Case 17 'xover_kind
'            g_diff = ((g_diff) / Abs(g_diff)) * 1
'            cmb_xover_kind.ListIndex = (g_xover_kind + 1) Mod 5
        
        Case 16 'xover_npt
            
            If cmb_xover_kind.ListIndex <> 2 Then
                g_test_count = 2
                ret = False
            Else
            
                g_diff = ((g_diff) / Abs(g_diff)) * 1
                txt_xover_npt.Text = Val(txt_xover_npt.Text) + g_diff
                If Val(txt_xover_npt) < 1 Then
                    txt_xover_npt = 1
                End If
                txt_xover_npt.FontBold = True
            End If
            
        Case 17 'chnage_xover_src_p
            
            If cmb_xover_kind.ListIndex <> 3 Then
                g_test_count = 2
                ret = False
            Else
            
                g_diff = ((g_diff) / Abs(g_diff)) * g_diff_p
                
                t = txt_change_xover_src_p.Text
                
                t = Val(t) + g_diff
                
                If Val(t) < 0 Then
                    t = 0
                End If
                If Val(t) > 1 Then
                    t = 1
                End If
            
                txt_change_xover_src_p = t
                txt_change_xover_src_p.FontBold = True
            End If
            

    
        Case 18  'xover_uniform_p
                        
            If cmb_xover_kind.ListIndex <> 4 Then
                g_test_count = 2
                ret = False
            Else
            
                g_diff = ((g_diff) / Abs(g_diff)) * g_diff_p
                
                t = txt_xover_uniform_p.Text
                
                t = Val(t) + g_diff
                
                If Val(t) < 0 Then
                    t = 0
                End If
                
                If Val(t) > 1 Then
                    t = 1
                End If
                
                txt_xover_uniform_p.Text = t
                txt_xover_uniform_p.FontBold = True
            End If
            
        Case 19 'do xover_p
            g_diff = ((g_diff) / Abs(g_diff)) * g_diff_p
 
            t = txt_do_xover_p.Text
            
            t = Val(t) + g_diff
            
            If Val(t) < 0 Then
                t = 0
            End If
            
            If Val(t) > 1 Then
                t = 1
            End If
            
            txt_do_xover_p = t
            txt_do_xover_p.FontBold = True
    
    End Select
    
    GoNextOptimize = ret
End Function


Private Sub ClearFontBold()
            txt_population_size.FontBold = False
            txt_generation_size.FontBold = False
            txt_tournament_winner_p.FontBold = False
            txt_replace_bad_p.FontBold = False
            txt_do_optimize_p.FontBold = False
            txt_local_optimize_count.FontBold = False
            txt_do_deep_better_optimize_p.FontBold = False
            txt_do_deep_worse_optimize_p.FontBold = False
            txt_do_spin_p.FontBold = False
            txt_spin_count.FontBold = False
            txt_do_spin_apply_p.FontBold = False
            txt_do_mutation1_p.FontBold = False
            txt_mutation1_p.FontBold = False
            txt_do_mutation2_p.FontBold = False
            txt_mutation2_p.FontBold = False
            chk_force_mutation2.FontBold = False
            txt_xover_npt.FontBold = False
            txt_change_xover_src_p.FontBold = False
            txt_xover_uniform_p.FontBold = False
            txt_do_xover_p.FontBold = False
End Sub

