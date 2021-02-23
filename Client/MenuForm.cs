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
    public partial class MenuForm : Form
    {
        public MenuForm()
        {
            InitializeComponent();
        }

        private void singlePlayButton_Click(object sender, EventArgs e)
        {
            Hide(); //singlePlayButton을 눌렀을 때 현재 창 숨겨서 보이지 않도록 처리
            //SinglePlayForm의 객체(인스턴스)를 만들기
            SinglePlayForm singlePlayForm = new SinglePlayForm();
            //singlePlayForm이 닫혔을때는 childForm_Closed를 이용해서 다시 menuForm 화면을 출력
            singlePlayForm.FormClosed += new FormClosedEventHandler(childForm_Closed);
            //show 함수를 불러옴으로서 새로운 창이 눈에 보이도록 처리
            singlePlayForm.Show();

        }

        private void exitButton_Click(object sender, EventArgs e)
        {
            //exitButton을 눌렀을때 프로그램 전체가 종료될 수 있도록 함
            //Exit()은 기본적으로 windows Form application library안에 존재
            System.Windows.Forms.Application.Exit();
        }

        //추가적으로 이벤트 처리 함수 생성
        //메뉴 화면에서 singlePlay화면으로 넘어갔다가 singlePlay화면이 닫혀서 다시 menu화면으로 돌아오고자 할때
        //childForm_closed 함수를 이용해서 eventHandler를 통해 처리
        void childForm_Closed(object sender, FormClosedEventArgs e)
        {
            //다시 menuForm 화면을 출력
            Show();
        }

        private void multiPlayButton_Click(object sender, EventArgs e)
        {
            Hide();
            MultiPlayForm multiPlayForm = new MultiPlayForm();
            multiPlayForm.FormClosed += new FormClosedEventHandler(childForm_Closed);
            multiPlayForm.Show();
        }

        private void AIPlayButton_Click(object sender, EventArgs e)
        {
            Hide();
            AIPlayForm aiPlayform = new AIPlayForm();
            aiPlayform.FormClosed += new FormClosedEventHandler(childForm_Closed);
            aiPlayform.Show();
        }
    }
}
