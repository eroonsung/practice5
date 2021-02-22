using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Client
{
    public partial class SinglePlayForm : Form
    {   

        private const int rectSize = 33; //오목판의 셀 크기
        private const int edgeCount = 15; // 오목판의 선 개수
        //오목판 15*15

        private enum Horse { none=0, BLACK, WHITE }; //특정한 오목판이 비어있다면 0의 값 가짐
        private Horse[,] board = new Horse[edgeCount, edgeCount]; // 15*15크기 형태의 2차원 배열 만듦
        private Horse nowPlayer = Horse.BLACK; // 현재 차례의 player, 기본적으로 검은 돌 먼저 게임 시작

        //변수 추가
        //현재 게임을 진행중인 상태인지 체크
        private bool playing = false;
        public SinglePlayForm()
        {
            InitializeComponent();
        }

        //승리 판정 함수(오목 형성 확인)
        private bool judge()
        {
            for (int i = 0; i < edgeCount - 4; i++) //가로
                for (int j = 0; j < edgeCount; j++)
                    if (board[i, j] == nowPlayer && board[i + 1, j] == nowPlayer && board[i + 2, j] == nowPlayer && board[i + 3, j] == nowPlayer && board[i + 4, j] == nowPlayer)
                        return true;
            for (int i = 0; i < edgeCount; i++) //세로
                for (int j = 0; j < edgeCount; j++)
                    if (board[i, j] == nowPlayer && board[i, j - 1] == nowPlayer && board[i, j - 2] == nowPlayer && board[i, j - 3] == nowPlayer && board[i, j - 4] == nowPlayer)
                        return true;
            for (int i = 0; i < edgeCount - 4; i++) //Y=X 직선, 대각선
                for (int j = 0; j < edgeCount - 4; j++)
                    if (board[i, j] == nowPlayer && board[i + 1, j] == nowPlayer && board[i + 2, j + 2] == nowPlayer && board[i + 3, j + 3] == nowPlayer && board[i + 4, j + 4] == nowPlayer)
                        return true;
            for (int i = 4; i < edgeCount; i++) //Y=-X 직선, 대각선
                for (int j = 0; j < edgeCount - 4; j++)
                    if (board[i, j] == nowPlayer && board[i - 1, j + 1] == nowPlayer && board[i - 2, j + 2] == nowPlayer && board[i - 3, j + 3] == nowPlayer && board[i - 4, j + 4] == nowPlayer)
                        return true;
            return false;
        }

        private void refresh()
        {
            this.boardPicture.Refresh();
            for (int i = 0; i < edgeCount; i++)
                for (int j = 0; j < edgeCount; j++)
                    board[i, j] = Horse.none;
        }

        private void SinglePlayForm_Load(object sender, EventArgs e)
        {

        }

        //오목판을 클릭했을 때 발생하는 이벤트
        private void boardPicture_MouseDown(object sender, MouseEventArgs e)
        {
            if (!playing)
            {
                MessageBox.Show("게임을 실행해 주세요.");
                return;
            }

            //오목판에 그림을 그리기 위해서 Graphics 객체를 만듦
            Graphics g = this.boardPicture.CreateGraphics();
            //현재 마우스 이벤트가 발생했을 때의 위치/ 오목판을 셀 크기로 나눔
            // => 정확히 현재 사용자가 클릭한 그 위치가 몇번째 셀인지를 확인할 수 있도록 처리
            int x = e.X / rectSize;
            int y = e.Y / rectSize;
            //모든 셀의 위치는 0~14
            if(x<0 || y<0 || x>=edgeCount || y>=edgeCount)
            {
                //그 범위를 벗어난다면 오류 메시지 출력
                MessageBox.Show("테두리를 벗어날 수 없습니다.");
                return;
            }
            //현재 돌을 놓고자 하는 위치가 정해졌을 때 그 위치에 어떠한 돌도 놓여져 있지 않아야지 해당 위치에 돌을 놓을 수 있게 함
            if (board[x, y] != Horse.none) return;
            board[x, y] = nowPlayer;

            if(nowPlayer == Horse.BLACK)
            {
                SolidBrush brush = new SolidBrush(Color.Black);
                //현재 그래픽 객체에 원형을 그릴 수 있도록 만듦
                g.FillEllipse(brush, x * rectSize, y * rectSize, rectSize, rectSize);
            }
            else
            {
                SolidBrush brush = new SolidBrush(Color.White);
                g.FillEllipse(brush, x * rectSize, y * rectSize, rectSize, rectSize);
            }
            //판정부분 추가
            //특정한 플레이어가 돌을 둔 이후에는 그 플레이어를 기점으로 해서 judge 함수를 불러와서 
            //해당 플레이어가 만약에 오목을 형성했다면 그 플레이어가 승리, 게임 더이상 동작하지 않음
            if (judge())
            {
                status.Text = nowPlayer.ToString() + "플레이어가 승리했습니다.";
                playing = false;
                playButton.Text = "게임시작";
            }
            else
            {
                //오목이 만들어지지 않은 상태라면 다른사람의 차례로 넘김
                nowPlayer = ((nowPlayer == Horse.BLACK) ? Horse.WHITE : Horse.BLACK);
                status.Text = nowPlayer.ToString() + " 플레이어의 차례입니다";
            }
        }

        //오목판이 처음에 어떻게 구성될 수 있는지 정의
        //오목판의 그림객체를 refresh 할 때마다 매번 다시 그려지기 때문에
        //어떠한 오목판과 같은 그래픽 요소의 초기화면을 구성할 때 사용할 수 있는 함수
        private void boardPicture_Paint(object sender, PaintEventArgs e)
        {
            Graphics gp = e.Graphics;
            Color lineColor = Color.Black; // 오목판의 선 색깔
            //pen이라는 변수를 이용하면 어떠한 선을 그릴때 굵기를 지정할 수 있음
            Pen p = new Pen(lineColor, 2);
            //전체 오목판의 테두리
            gp.DrawLine(p, rectSize / 2, rectSize / 2, rectSize / 2, rectSize * edgeCount - rectSize / 2);//좌측
            gp.DrawLine(p, rectSize / 2, rectSize / 2, rectSize * edgeCount - rectSize / 2, rectSize / 2);//상측
            gp.DrawLine(p, rectSize / 2, rectSize * edgeCount - rectSize / 2, rectSize * edgeCount - rectSize / 2, rectSize * edgeCount - rectSize / 2);//하측
            gp.DrawLine(p, rectSize * edgeCount - rectSize / 2, rectSize / 2, rectSize * edgeCount - rectSize / 2, rectSize * edgeCount - rectSize / 2);//우측
            p = new Pen(lineColor, 1);
            //오목판의 내부 - 대각선 방향으로 반복적으로 이동하면서 십자가 모양의 선 그리기
            for (int i = rectSize + rectSize / 2; i < rectSize * edgeCount - rectSize / 2; i += rectSize)
            {
                gp.DrawLine(p, rectSize / 2, i, rectSize * edgeCount - rectSize / 2, i);
                gp.DrawLine(p, i, rectSize / 2, i, rectSize * edgeCount - rectSize / 2);
            }
        }

        private void label1_Click(object sender, EventArgs e)
        {

        }

        private void playButton_Click(object sender, EventArgs e)
        {
            //현재 게임이 진행되는 중이 아닐 때
            if (!playing)
            {
                //화면 초기화
                refresh();
                //게임 시작중인 상태로 바꿈
                playing = true;
                //버튼을 재시작으로 만듦
                playButton.Text = "재시작";
                //현재 상태메시지 출력
                status.Text = nowPlayer.ToString() + " 플레이어의 차례입니다.";
            }
            else
            {
                // 현재 게임이 실행되는 중
                refresh();
                status.Text = "게임이 재시작되었습니다";
            }
        }
    }
}
