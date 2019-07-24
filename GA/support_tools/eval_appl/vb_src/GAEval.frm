VERSION 5.00
Begin VB.Form GAEval 
   Caption         =   "GA평가"
   ClientHeight    =   6360
   ClientLeft      =   60
   ClientTop       =   345
   ClientWidth     =   4650
   LinkTopic       =   "Form1"
   ScaleHeight     =   6360
   ScaleWidth      =   4650
   StartUpPosition =   3  'Windows 기본값
   Begin VB.CommandButton Command4 
      Caption         =   "합의 차이 또는 표준편차 계산"
      Height          =   360
      Left            =   1590
      TabIndex        =   9
      Top             =   5910
      Width           =   2985
   End
   Begin VB.TextBox Text4 
      Height          =   1080
      Left            =   105
      MultiLine       =   -1  'True
      ScrollBars      =   2  '수직
      TabIndex        =   8
      Top             =   4710
      Width           =   4455
   End
   Begin VB.CommandButton Command3 
      Caption         =   "각 그룹별 합 계산"
      Height          =   375
      Left            =   1590
      TabIndex        =   7
      Top             =   4170
      Width           =   2955
   End
   Begin VB.TextBox Text3 
      Height          =   1080
      Left            =   90
      MultiLine       =   -1  'True
      ScrollBars      =   2  '수직
      TabIndex        =   6
      Top             =   2940
      Width           =   4455
   End
   Begin VB.TextBox Text1 
      Height          =   1575
      Left            =   1125
      Locked          =   -1  'True
      MultiLine       =   -1  'True
      ScrollBars      =   2  '수직
      TabIndex        =   3
      Top             =   465
      Width           =   2055
   End
   Begin VB.TextBox Text2 
      Height          =   285
      Left            =   1125
      TabIndex        =   1
      Top             =   60
      Width           =   2070
   End
   Begin VB.CommandButton Command1 
      Caption         =   "Read Data"
      Height          =   330
      Left            =   3240
      TabIndex        =   0
      Top             =   45
      Width           =   1215
   End
   Begin VB.Label Label4 
      Height          =   270
      Left            =   120
      TabIndex        =   10
      Top             =   5895
      Width           =   1395
   End
   Begin VB.Label Label3 
      Caption         =   "chromosome"
      Height          =   225
      Left            =   90
      TabIndex        =   5
      Top             =   2730
      Width           =   1200
   End
   Begin VB.Label Label2 
      Height          =   315
      Left            =   1140
      TabIndex        =   4
      Top             =   2130
      Width           =   2085
   End
   Begin VB.Label Label1 
      Caption         =   "problemset"
      Height          =   225
      Left            =   120
      TabIndex        =   2
      Top             =   105
      Width           =   990
   End
End
Attribute VB_Name = "GAEval"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Dim data(5000) As Double
Dim g_count As Integer
Dim Sum(1000) As Double
Dim g_max As Integer

Private Sub Command1_Click()
    g_max = 0
    g_count = 0
    Dim j As Integer
    
    For j = 0 To 1000
        Sum(j) = 0
    Next j
    
    For j = 0 To 5000
        data(j) = 0
    Next j
        
    MousePointer = 11
    On Error GoTo Err_Handler
    
    Dim t As Variant
    g_count = 0
    Text1.Text = ""
    
    Open Text2.Text For Input As #1
    
    Do While Not EOF(1)
    
        Input #1, t
        data(g_count) = Val(t)
        g_count = g_count + 1
        
        If g_count < 5 Then
            Text1.Text = Text1.Text & t & vbCrLf
        End If
        
        If g_count >= 5 And g_count < 8 Then
            Text1.Text = Text1.Text & "." & vbCrLf
        End If
    Loop
    
    Text1.Text = Text1.Text & data(g_count - 1)
    Close #1
    Label2.Caption = "총 자료의 개수: " & g_count & "개"
    MsgBox g_count & "개의 자료가 입력되었습니다."
    MousePointer = 0
    
    Exit Sub
    
Err_Handler:
    MsgBox "입력이 잘못되었습니다", vbCritical, "입력오류"
    
    MousePointer = 0
    
End Sub

Private Sub Command3_Click()
            
    Text4.Text = ""
    
    MousePointer = 11

    On Error GoTo Err_Handler2
    
    Dim i As Integer
    Dim max As Integer
    
    Dim index As Integer
    
    max = 0
    
    For i = 0 To 1000
        Sum(i) = 0
    Next i
    
    For i = 0 To g_count
        
        index = Val(Mid(Text3.Text, i + 1, 1))
        Sum(index) = Sum(index) + data(i)
        If max < index Then
            max = index
        End If
    Next i
    
    For i = 0 To max
        Text4.Text = Text4.Text & Sum(i) & vbCrLf
    Next i
    
    g_max = max
    Label4.Caption = "총 " & g_max + 1 & "개 그룹"
    
    MsgBox "총 " & g_max + 1 & "개 그룹으로 나누어졌습니다", vbInformation, "완료"
    MousePointer = 0
    
    Exit Sub
    
Err_Handler2:
    MsgBox "입력이 잘못되었습니다", vbCritical, "입력오류"
    MousePointer = 0
End Sub

Private Sub Command4_Click()
        
    On Error GoTo Err_Handler3
    MousePointer = 11
    
    Dim i As Integer
    Dim total_sum As Double
    total_sum = 0
    Dim avg As Double
    Dim str_temp As String
    
    Dim sd As Double
    
    If g_max > 1 Then
    
        For i = 0 To g_max
           total_sum = total_sum + Sum(i)
        Next i
        
        avg = total_sum / (g_max + 1)
        
        sd = 0
        For i = 0 To g_max
            sd = sd + (Sum(i) - avg) * (Sum(i) - avg)
        Next i
        
        sd = sd / (g_max + 1)
        
        sd = Sqr(sd)
                
        str_temp = "표준편차는 "
    Else
        sd = Sum(1) - Sum(0)
        If sd < 0 Then sd = -sd
        
        str_temp = "합의 차이는 "
    End If
    
    MsgBox str_temp & sd & " 입니다.", vbInformation, "결과"
    MousePointer = 0
        
    Exit Sub
Err_Handler3:
    MsgBox "오류가 발생했습니다", vbCritical, "오류"
    MousePointer = 0
End Sub

Private Sub Form_Load()
    Text2.Text = GetSetting("ga_eval", "config", "filepath", "C:\GA\Release\problem5000")
    
End Sub

Private Sub Form_Unload(Cancel As Integer)
    Call SaveSetting("ga_eval", "config", "filepath", Text2.Text)
    
End Sub
