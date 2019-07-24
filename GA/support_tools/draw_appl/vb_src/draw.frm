VERSION 5.00
Begin VB.Form FormDraw 
   Caption         =   "Draw GA graph"
   ClientHeight    =   8655
   ClientLeft      =   60
   ClientTop       =   345
   ClientWidth     =   12270
   LinkTopic       =   "Form1"
   ScaleHeight     =   8655
   ScaleWidth      =   12270
   StartUpPosition =   3  'Windows 기본값
   Begin VB.TextBox Text5 
      Height          =   270
      Left            =   8895
      TabIndex        =   10
      Top             =   75
      Width           =   660
   End
   Begin VB.TextBox Text4 
      Height          =   270
      Left            =   7065
      TabIndex        =   8
      Top             =   75
      Width           =   660
   End
   Begin VB.TextBox Text3 
      Height          =   270
      Left            =   645
      TabIndex        =   6
      Top             =   75
      Width           =   2205
   End
   Begin VB.TextBox Text2 
      Height          =   270
      Left            =   5550
      TabIndex        =   4
      Top             =   75
      Width           =   660
   End
   Begin VB.TextBox Text1 
      Height          =   270
      Left            =   3825
      TabIndex        =   2
      Top             =   90
      Width           =   885
   End
   Begin VB.CommandButton Command1 
      Caption         =   "Start"
      Height          =   330
      Left            =   10020
      TabIndex        =   1
      Top             =   75
      Width           =   1215
   End
   Begin VB.PictureBox Picture1 
      AutoRedraw      =   -1  'True
      BackColor       =   &H80000009&
      BorderStyle     =   0  '없음
      Height          =   8130
      Left            =   15
      ScaleHeight     =   8130
      ScaleWidth      =   12210
      TabIndex        =   0
      Top             =   480
      Width           =   12210
   End
   Begin VB.Label Label5 
      Caption         =   "repl_bad_r"
      Height          =   225
      Left            =   7860
      TabIndex        =   11
      Top             =   105
      Width           =   975
   End
   Begin VB.Label Label4 
      Caption         =   "Mut_r"
      Height          =   225
      Left            =   6420
      TabIndex        =   9
      Top             =   120
      Width           =   645
   End
   Begin VB.Label Label3 
      Caption         =   "File"
      Height          =   225
      Left            =   135
      TabIndex        =   7
      Top             =   105
      Width           =   540
   End
   Begin VB.Label Label2 
      Caption         =   "Max sd"
      Height          =   225
      Left            =   4830
      TabIndex        =   5
      Top             =   120
      Width           =   930
   End
   Begin VB.Label Label1 
      Caption         =   "Max Gen"
      Height          =   225
      Left            =   2940
      TabIndex        =   3
      Top             =   120
      Width           =   870
   End
End
Attribute VB_Name = "FormDraw"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
'Option Explicit

Private Sub Command1_Click()

    Picture1.Cls

    On Error GoTo err_handler
    
    Dim gen_num As Currency
    Dim sd As Double
    Dim average As Double
    Dim sd_percent As Double
    Dim mutation_r As Double
    Dim replace_bad_r As Double
    
    Dim old_gen_num As Currency
    Dim old_sd As Double
    Dim old_average As Double
    Dim old_sd_percent As Double
    Dim old_mutation_r As Double
    Dim old_replace_bad_r As Double
    
    Dim t_string
    Dim t As Variant
    
    Dim max_gen_num As Currency
    Dim max_sd As Double
    
    Dim max_mutation_r As Double
    Dim max_replace_bad_r As Double
    
    max_gen_num = Val(Text1.Text)
    
    max_sd = Val(Text2.Text)
    max_mutation_r = Val(Text4.Text)
    max_replace_bad_r = Val(Text5.Text)
    
    old_gen_num = 0
    
    Open Text3.Text For Input As #1
    
    
    '필요없는 값 제거
    Input #1, t
    Debug.Print t_string
    
    Input #1, t
    Debug.Print t_string
    
    Input #1, t
    Debug.Print t_string
    
    Input #1, t
    
    Debug.Print t_string
    
    Input #1, t
    
    Debug.Print t_string
    
    Input #1, t
    
    Debug.Print t_string
    
    '자료 시작
    Do While Not EOF(1)
    
    Input #1, gen_num, sd, average, sd_percent, mutation_r, replace_bad_r
    
    On Error Resume Next
    
    If old_gen_num = 0 Then
        Picture1.PSet (Picture1.ScaleWidth * (gen_num / max_gen_num), Picture1.ScaleHeight * (1 - sd / max_sd)), RGB(150, 150, 150)
        Picture1.PSet (Picture1.ScaleWidth * (gen_num / max_gen_num), Picture1.ScaleHeight * (1 - average / max_sd)), vbBlue
        
        Picture1.PSet (Picture1.ScaleWidth * (gen_num / max_gen_num), Picture1.ScaleHeight * (1 - sd_percent)), vbGreen
        Picture1.PSet (Picture1.ScaleWidth * (gen_num / max_gen_num), Picture1.ScaleHeight * (1 - (mutation_r / max_mutation_r))), RGB(200, 200, 0)
        Picture1.PSet (Picture1.ScaleWidth * (gen_num / max_gen_num), Picture1.ScaleHeight * (1 - (mutation_r / max_replace_bad_r))), RGB(0, 200, 200)
        
    Else
        Picture1.Line (Picture1.ScaleWidth * (gen_num / max_gen_num), Picture1.ScaleHeight * (1 - sd / max_sd))-(Picture1.ScaleWidth * (old_gen_num / max_gen_num), Picture1.ScaleHeight * (1 - old_sd / max_sd)), vbBlue
        Picture1.Line (Picture1.ScaleWidth * (gen_num / max_gen_num), Picture1.ScaleHeight * (1 - average / max_sd))-(Picture1.ScaleWidth * (old_gen_num / max_gen_num), Picture1.ScaleHeight * (1 - old_average / max_sd)), vbRed
    
        Picture1.Line (Picture1.ScaleWidth * (gen_num / max_gen_num), Picture1.ScaleHeight * (1 - sd_percent))-(Picture1.ScaleWidth * (old_gen_num / max_gen_num), Picture1.ScaleHeight * (1 - old_sd_percent)), vbGreen
        Picture1.Line (Picture1.ScaleWidth * (gen_num / max_gen_num), Picture1.ScaleHeight * (1 - (mutation_r / max_mutation_r)))-(Picture1.ScaleWidth * (old_gen_num / max_gen_num), Picture1.ScaleHeight * (1 - (old_mutation_r / max_mutation_r))), RGB(200, 200, 0)
        Picture1.Line (Picture1.ScaleWidth * (gen_num / max_gen_num), Picture1.ScaleHeight * (1 - (replace_bad_r / max_replace_bad_r)))-(Picture1.ScaleWidth * (old_gen_num / max_gen_num), Picture1.ScaleHeight * (1 - (old_replace_bad_r / max_replace_bad_r))), RGB(0, 200, 200)
    
    End If
    
    old_gen_num = gen_num
    old_sd = sd
    old_average = average
    old_sd_percent = sd_percent
    old_mutation_r = mutation_r
    old_replace_bad_r = replace_bad_r
    
    Loop
    
    Close #1
    Exit Sub
    
err_handler:
    
    MsgBox "입력이 잘못되었습니다.", vbCritical, "입력오류"
    Exit Sub
End Sub

Private Sub Form_Load()
    Me.WindowState = 2
End Sub

Private Sub Form_Resize()
    
    On Error Resume Next
    
    Picture1.Left = 15
    Picture1.Top = 480
    Picture1.Width = FormDraw.ScaleWidth - 30
    Picture1.Height = FormDraw.ScaleHeight - 500
    
End Sub
