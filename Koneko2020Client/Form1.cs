using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Threading;
using System.Windows.Forms;
using Newtonsoft.Json;

namespace Koneko2020Client
{
    public partial class Form1 : Form
    {
        System.Threading.Timer timer;
        int clock;
        TextBox[,] cells = new TextBox[7, 7];
        int nowRow;
        int nowCol;
        string ans = string.Empty;

        System.Net.Sockets.TcpClient tcp;
        System.Net.Sockets.NetworkStream ns;

        string serverNotification;

        int myPoint;
        int opPoint;

        private void CommunicationStart()
        {
            //サーバーのIPアドレス（または、ホスト名）とポート番号
            string ipOrHost = "10.25.222.212";
            //string ipOrHost = "localhost";
            int port = 54949;

            try
            {
                //TcpClientを作成し、サーバーと接続する
                tcp = new System.Net.Sockets.TcpClient(ipOrHost, port);

                //NetworkStreamを取得する
                ns = tcp.GetStream();
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }
        }

        private void CommunicationEnd()
        {
            //閉じる
            ns.Close();
            tcp.Close();

        }

        public Form1()
        {
            InitializeComponent();

            // サーバ接続クライアントの構築、接続要求を出す
            CommunicationStart();

            cells[0, 0] = textBox1;
            cells[0, 1] = textBox2;
            cells[0, 2] = textBox3;
            cells[0, 3] = textBox4;
            cells[0, 4] = textBox5;
            cells[0, 5] = textBox6;
            cells[0, 6] = textBox7;


            cells[1, 0] = textBox8;
            cells[1, 1] = textBox9;
            cells[1, 2] = textBox10;
            cells[1, 3] = textBox11;
            cells[1, 4] = textBox12;
            cells[1, 5] = textBox13;
            cells[1, 6] = textBox14;


            cells[2, 0] = textBox15;
            cells[2, 1] = textBox16;
            cells[2, 2] = textBox17;
            cells[2, 3] = textBox18;
            cells[2, 4] = textBox19;
            cells[2, 5] = textBox20;
            cells[2, 6] = textBox21;


            cells[3, 0] = textBox22;
            cells[3, 1] = textBox23;
            cells[3, 2] = textBox24;
            cells[3, 3] = textBox25;
            cells[3, 4] = textBox26;
            cells[3, 5] = textBox27;
            cells[3, 6] = textBox28;


            cells[4, 0] = textBox29;
            cells[4, 1] = textBox30;
            cells[4, 2] = textBox31;
            cells[4, 3] = textBox32;
            cells[4, 4] = textBox33;
            cells[4, 5] = textBox34;
            cells[4, 6] = textBox35;


            cells[5, 0] = textBox36;
            cells[5, 1] = textBox37;
            cells[5, 2] = textBox38;
            cells[5, 3] = textBox39;
            cells[5, 4] = textBox40;
            cells[5, 5] = textBox41;
            cells[5, 6] = textBox42;


            cells[6, 0] = textBox43;
            cells[6, 1] = textBox44;
            cells[6, 2] = textBox45;
            cells[6, 3] = textBox46;
            cells[6, 4] = textBox47;
            cells[6, 5] = textBox48;
            cells[6, 6] = textBox49;


            cells[0, 0].MouseDown += TextBox1_TextChanged;
            cells[0, 1].MouseDown += TextBox2_TextChanged;
            cells[0, 2].MouseDown += TextBox3_TextChanged;
            cells[0, 3].MouseDown += TextBox4_TextChanged;
            cells[0, 4].MouseDown += TextBox5_TextChanged;
            cells[0, 5].MouseDown += TextBox6_TextChanged;
            cells[0, 6].MouseDown += TextBox7_TextChanged;

            cells[1, 0].MouseDown += TextBox8_TextChanged;
            cells[1, 1].MouseDown += TextBox9_TextChanged;
            cells[1, 2].MouseDown += TextBox10_TextChanged;
            cells[1, 3].MouseDown += TextBox11_TextChanged;
            cells[1, 4].MouseDown += TextBox12_TextChanged;
            cells[1, 5].MouseDown += TextBox13_TextChanged;
            cells[1, 6].MouseDown += TextBox14_TextChanged;

            cells[2, 0].MouseDown += TextBox15_TextChanged;
            cells[2, 1].MouseDown += TextBox16_TextChanged;
            cells[2, 2].MouseDown += TextBox17_TextChanged;
            cells[2, 3].MouseDown += TextBox18_TextChanged;
            cells[2, 4].MouseDown += TextBox19_TextChanged;
            cells[2, 5].MouseDown += TextBox20_TextChanged;
            cells[2, 6].MouseDown += TextBox21_TextChanged;

            cells[3, 0].MouseDown += TextBox22_TextChanged;
            cells[3, 1].MouseDown += TextBox23_TextChanged;
            cells[3, 2].MouseDown += TextBox24_TextChanged;
            cells[3, 3].MouseDown += TextBox25_TextChanged;
            cells[3, 4].MouseDown += TextBox26_TextChanged;
            cells[3, 5].MouseDown += TextBox27_TextChanged;
            cells[3, 6].MouseDown += TextBox28_TextChanged;

            cells[4, 0].MouseDown += TextBox29_TextChanged;
            cells[4, 1].MouseDown += TextBox30_TextChanged;
            cells[4, 2].MouseDown += TextBox31_TextChanged;
            cells[4, 3].MouseDown += TextBox32_TextChanged;
            cells[4, 4].MouseDown += TextBox33_TextChanged;
            cells[4, 5].MouseDown += TextBox34_TextChanged;
            cells[4, 6].MouseDown += TextBox35_TextChanged;

            cells[5, 0].MouseDown += TextBox36_TextChanged;
            cells[5, 1].MouseDown += TextBox37_TextChanged;
            cells[5, 2].MouseDown += TextBox38_TextChanged;
            cells[5, 3].MouseDown += TextBox39_TextChanged;
            cells[5, 4].MouseDown += TextBox40_TextChanged;
            cells[5, 5].MouseDown += TextBox41_TextChanged;
            cells[5, 6].MouseDown += TextBox42_TextChanged;

            cells[6, 0].MouseDown += TextBox43_TextChanged;
            cells[6, 1].MouseDown += TextBox44_TextChanged;
            cells[6, 2].MouseDown += TextBox45_TextChanged;
            cells[6, 3].MouseDown += TextBox46_TextChanged;
            cells[6, 4].MouseDown += TextBox47_TextChanged;
            cells[6, 5].MouseDown += TextBox48_TextChanged;
            cells[6, 6].MouseDown += TextBox49_TextChanged;

            button1.Focus();

            myPoint = 0;
            opPoint = 0;

            clock = 60000;

            Task.Run(() => ServerNotificationCommunicatorFirst("match_request", -1, -1, ans));
        }

        private void TextBox49_TextChanged(object sender, EventArgs e)
        {
            nowRow = 6;
            nowCol = 6;

            CommonFunction(nowRow, nowCol);
        }

        private void TextBox48_TextChanged(object sender, EventArgs e)
        {
            nowRow = 6;
            nowCol = 5;

            CommonFunction(nowRow, nowCol);
        }

        private void TextBox47_TextChanged(object sender, EventArgs e)
        {
            nowRow = 6;
            nowCol = 4;

            CommonFunction(nowRow, nowCol);
        }

        private void TextBox46_TextChanged(object sender, EventArgs e)
        {
            nowRow = 6;
            nowCol = 3;

            CommonFunction(nowRow, nowCol);
        }

        private void TextBox45_TextChanged(object sender, EventArgs e)
        {
            nowRow = 6;
            nowCol = 2;

            CommonFunction(nowRow, nowCol);
        }

        private void TextBox44_TextChanged(object sender, EventArgs e)
        {
            nowRow = 6;
            nowCol = 1;

            CommonFunction(nowRow, nowCol);
        }

        private void TextBox43_TextChanged(object sender, EventArgs e)
        {
            nowRow = 6;
            nowCol = 0;

            CommonFunction(nowRow, nowCol);
        }

        private void TextBox42_TextChanged(object sender, EventArgs e)
        {
            nowRow = 5;
            nowCol = 6;

            CommonFunction(nowRow, nowCol);
        }

        private void TextBox41_TextChanged(object sender, EventArgs e)
        {
            nowRow = 5;
            nowCol = 5;

            CommonFunction(nowRow, nowCol);
        }

        private void TextBox40_TextChanged(object sender, EventArgs e)
        {
            nowRow = 5;
            nowCol = 4;

            CommonFunction(nowRow, nowCol);
        }

        private void TextBox39_TextChanged(object sender, EventArgs e)
        {
            nowRow = 5;
            nowCol = 3;

            CommonFunction(nowRow, nowCol);
        }

        private void TextBox38_TextChanged(object sender, EventArgs e)
        {
            nowRow = 5;
            nowCol = 2;

            CommonFunction(nowRow, nowCol);
        }

        private void TextBox37_TextChanged(object sender, EventArgs e)
        {
            nowRow = 5;
            nowCol = 1;

            CommonFunction(nowRow, nowCol);
        }

        private void TextBox36_TextChanged(object sender, EventArgs e)
        {
            nowRow = 5;
            nowCol = 0;

            CommonFunction(nowRow, nowCol);
        }

        private void TextBox35_TextChanged(object sender, EventArgs e)
        {
            nowRow = 4;
            nowCol = 6;

            CommonFunction(nowRow, nowCol);
        }

        private void TextBox34_TextChanged(object sender, EventArgs e)
        {
            nowRow = 4;
            nowCol = 5;

            CommonFunction(nowRow, nowCol);
        }

        private void TextBox33_TextChanged(object sender, EventArgs e)
        {
            nowRow = 4;
            nowCol = 4;

            CommonFunction(nowRow, nowCol);
        }

        private void TextBox32_TextChanged(object sender, EventArgs e)
        {
            nowRow = 4;
            nowCol = 3;

            CommonFunction(nowRow, nowCol);
        }

        private void TextBox31_TextChanged(object sender, EventArgs e)
        {
            nowRow = 4;
            nowCol = 2;

            CommonFunction(nowRow, nowCol);
        }

        private void TextBox30_TextChanged(object sender, EventArgs e)
        {
            nowRow = 4;
            nowCol = 1;

            CommonFunction(nowRow, nowCol);
        }

        private void TextBox29_TextChanged(object sender, EventArgs e)
        {
            nowRow = 4;
            nowCol = 0;

            CommonFunction(nowRow, nowCol);
        }

        private void TextBox28_TextChanged(object sender, EventArgs e)
        {
            nowRow = 3;
            nowCol = 6;

            CommonFunction(nowRow, nowCol);
        }

        private void TextBox27_TextChanged(object sender, EventArgs e)
        {
            nowRow = 3;
            nowCol = 5;

            CommonFunction(nowRow, nowCol);
        }

        private void TextBox26_TextChanged(object sender, EventArgs e)
        {
            nowRow = 3;
            nowCol = 4;

            CommonFunction(nowRow, nowCol);
        }

        private void TextBox25_TextChanged(object sender, EventArgs e)
        {
            nowRow = 3;
            nowCol = 3;

            CommonFunction(nowRow, nowCol);
        }

        private void TextBox24_TextChanged(object sender, EventArgs e)
        {
            nowRow = 3;
            nowCol = 2;

            CommonFunction(nowRow, nowCol);
        }

        private void TextBox23_TextChanged(object sender, EventArgs e)
        {
            nowRow = 3;
            nowCol = 1;

            CommonFunction(nowRow, nowCol);
        }

        private void TextBox22_TextChanged(object sender, EventArgs e)
        {
            nowRow = 3;
            nowCol = 0;

            CommonFunction(nowRow, nowCol);
        }

        private void TextBox21_TextChanged(object sender, EventArgs e)
        {
            nowRow = 2;
            nowCol = 6;

            CommonFunction(nowRow, nowCol);
        }

        private void TextBox20_TextChanged(object sender, EventArgs e)
        {
            nowRow = 2;
            nowCol = 5;

            CommonFunction(nowRow, nowCol);
        }

        private void TextBox19_TextChanged(object sender, EventArgs e)
        {
            nowRow = 2;
            nowCol = 4;

            CommonFunction(nowRow, nowCol);
        }

        private void TextBox18_TextChanged(object sender, EventArgs e)
        {
            nowRow = 2;
            nowCol = 3;

            CommonFunction(nowRow, nowCol);
        }

        private void TextBox17_TextChanged(object sender, EventArgs e)
        {
            nowRow = 2;
            nowCol = 2;

            CommonFunction(nowRow, nowCol);
        }

        private void TextBox16_TextChanged(object sender, EventArgs e)
        {
            nowRow = 2;
            nowCol = 1;

            CommonFunction(nowRow, nowCol);
        }

        private void TextBox15_TextChanged(object sender, EventArgs e)
        {
            nowRow = 2;
            nowCol = 0;

            CommonFunction(nowRow, nowCol);
        }

        private void TextBox14_TextChanged(object sender, EventArgs e)
        {
            nowRow = 1;
            nowCol = 6;

            CommonFunction(nowRow, nowCol);
        }

        private void TextBox13_TextChanged(object sender, EventArgs e)
        {
            nowRow = 1;
            nowCol = 5;

            CommonFunction(nowRow, nowCol);
        }

        private void TextBox12_TextChanged(object sender, EventArgs e)
        {
            nowRow = 1;
            nowCol = 4;

            CommonFunction(nowRow, nowCol);
        }

        private void TextBox11_TextChanged(object sender, EventArgs e)
        {
            nowRow = 1;
            nowCol = 3;

            CommonFunction(nowRow, nowCol);
        }

        private void TextBox10_TextChanged(object sender, EventArgs e)
        {
            nowRow = 1;
            nowCol = 2;

            CommonFunction(nowRow, nowCol);
        }

        private void TextBox9_TextChanged(object sender, EventArgs e)
        {
            nowRow = 1;
            nowCol = 1;

            CommonFunction(nowRow, nowCol);
        }

        private void TextBox8_TextChanged(object sender, EventArgs e)
        {
            nowRow = 1;
            nowCol = 0;

            CommonFunction(nowRow, nowCol);
        }

        private void TextBox7_TextChanged(object sender, EventArgs e)
        {
            nowRow = 0;
            nowCol = 6;

            CommonFunction(nowRow, nowCol);
        }

        private void TextBox6_TextChanged(object sender, EventArgs e)
        {
            nowRow = 0;
            nowCol = 5;

            CommonFunction(nowRow, nowCol);
        }

        private void TextBox5_TextChanged(object sender, EventArgs e)
        {
            nowRow = 0;
            nowCol = 4;

            CommonFunction(nowRow, nowCol);
        }

        private void TextBox4_TextChanged(object sender, EventArgs e)
        {
            nowRow = 0;
            nowCol = 3;

            CommonFunction(nowRow, nowCol);
        }

        private void TextBox3_TextChanged(object sender, EventArgs e)
        {
            nowRow = 0;
            nowCol = 2;

            CommonFunction(nowRow, nowCol);
        }

        private void TextBox2_TextChanged(object sender, EventArgs e)
        {
            nowRow = 0;
            nowCol = 1;

            CommonFunction(nowRow, nowCol);
        }

        private void TextBox1_TextChanged(object sender, EventArgs e)
        {
            nowRow = 0;
            nowCol = 0;

            CommonFunction(nowRow, nowCol);
        }

        private async void CommonFunction(int row, int col)
        {
            if(true == cells[row, col].ReadOnly)
            {
                return;
            }

            Form2 f2 = new Form2();

            var result = f2.ShowDialog();

            if (result == DialogResult.OK)
            {
                ans = f2.ans;

                f2.Dispose();
            }

            cells[row, col].Text = ans;
            cells[row, col].ReadOnly = true;
            cells[row, col].BackColor = Color.LightPink;

            timer.Change(Timeout.Infinite, Timeout.Infinite);

            if ("こ" == ans)
            {
                ans = "ko";
            }
            else if ("ね" == ans)
            {
                ans = "ne";
            }

            var res = await Task.Run(() => ServerNotificationCommunicator("sashite", row, col, ans));
        }

        private void button1_Click(object sender, EventArgs e)
        {
            timer = new System.Threading.Timer(new TimerCallback(Timer_Tick), null, 0, 1);
        }

        private void Timer_Tick(object o)
        {
            clock--;

            label1.BeginInvoke(new Action(() => {

                int sec = 0;
                int min = 0;
                int milli = 0;

                try
                {
                    sec = (clock / 100) % 60;
                    min = clock / 6000;
                    milli = clock % 100;
                }
                catch(Exception ex)
                {
                    sec = 0;
                    min = 0;
                    milli = 0;
                }
                
                string str = string.Format("{0:00}:{1:00}:{2:00}", min, sec, milli);                

                label1.Text = str; 
            }));
        }

        private string SendMessageCreator(string command, int row, int col, string value)
        {
            string json = string.Empty;

            switch (command)
            {
                case "match_request":
                    json = "{{\"request\":\"match request\"}}";
                    break;
                case "OK":
                    json = "{{\"request\":\"OK\"}}";
                    break;
                case "sashite":
                    json = string.Format("{{\"sashite\":{{\"row\":{0},\"col\":{1},\"value\":\"{2}\"}}}}", row, col, value);
                    break;
            }

            return json;
        }

        // 受信サーバ
        private string ServerNotificationCommunicatorFirst(string command, int row, int col, string value)
        {
            string sendMsg = string.Empty;

            sendMsg = SendMessageCreator(command, row, col, value);

            //NetworkStreamを取得
            //サーバーにデータを送信する
            //文字列をByte型配列に変換
            System.Text.Encoding enc = System.Text.Encoding.UTF8;
            byte[] sendBytes = enc.GetBytes(sendMsg);
            //データを送信する
            ns.Write(sendBytes, 0, sendBytes.Length);
            //Console.WriteLine(sendMsg);

            //Thread.Sleep(1000);

            //サーバーから送られたデータを受信する
            System.IO.MemoryStream ms = new System.IO.MemoryStream();
            byte[] resBytes = new byte[256];
            int resSize = 0;
            do
            {
                //データの一部を受信する
                resSize = ns.Read(resBytes, 0, resBytes.Length);
                //Readが0を返した時はサーバーが切断したと判断
                if (resSize == 0)
                {
                    Console.WriteLine("サーバーが切断しました。");
                    break;
                }
                //受信したデータを蓄積する
                ms.Write(resBytes, 0, resSize);
                //まだ読み取れるデータがあるか、データの最後が\nでない時は、
                // 受信を続ける
            } while (ns.DataAvailable);
            //受信したデータを文字列に変換
            string resMsg = enc.GetString(ms.GetBuffer(), 0, (int)ms.Length);
            //string resMsg = Encoding.UTF8.GetString(resBytes);
            ms.Close();
            //末尾の\nを削除
            resMsg = resMsg.TrimEnd('\n');
            //Console.WriteLine(resMsg);







            //Json パーサーの移植

            Json json = new Json();

            StringBuilder element = new StringBuilder(1024);

            json.ParseJson(resMsg, out element);

            //MessageBox.Show(json.pair[10].key + json.pair[11].value);
            int l = 0;
            while (l < json.pair.Count)
            {
                var h = json.pair[l].value;
                Console.WriteLine("key = " + json.pair[l].key + " : value = " + h);
                l++;
            }

            l = 0;
            while(l<json.pair.Count)
            {
                string r = string.Empty;
                string c = string.Empty;
                string v = string.Empty;
                string p = string.Empty;

                if ("row" == json.pair[l].key)
                {
                    r = json.pair[l].value;
                    l++;
                    if ("col" == json.pair[l].key)
                    {
                        c = json.pair[l].value;
                        l++;
                        if ("value" == json.pair[l].key)
                        {
                            v = json.pair[l].value;
                            if ("\"ne\"" == v)
                            {
                                v = "ね";
                            }
                            else
                            {
                                v = "こ";
                            }

                            SetDefaultBoard(int.Parse(r), int.Parse(c), v);
                        }
                    }
                }

                l++;
            }

            
            resMsg = "OK";


            sendMsg = "{\"waiting\":\"OK\"}";

            sendBytes = enc.GetBytes(sendMsg);

            ns.Write(sendBytes, 0, sendBytes.Length);

            ms = new System.IO.MemoryStream();
            resBytes = new byte[256];
            resSize = 0;

            do
            {
                resSize = ns.Read(resBytes, 0, resBytes.Length);

                if (resSize == 0)
                {
                    Console.WriteLine("サーバーが切断しました。");
                    break;
                }

                ms.Write(resBytes, 0, resSize);

            } while (ns.DataAvailable);

            resMsg = enc.GetString(ms.GetBuffer(), 0, (int)ms.Length);
            ms.Close();

            resMsg = resMsg.TrimEnd('\n');

            Console.WriteLine(resMsg);


            

            if (resMsg == "OK")
            {
                //MessageBox.Show("OK");

                // グローバルの通知専用変数に代入

                timer = new System.Threading.Timer(new TimerCallback(Timer_Tick), null, 0, 1);

                serverNotification = "start";

                return "Hello";

            }
            else if (resMsg == "disconnect")
            {
                //MessageBox.Show("disconnect");

                timer.Change(Timeout.Infinite, Timeout.Infinite);

                CommunicationEnd();

                return "Close";
            }

            timer = new System.Threading.Timer(new TimerCallback(Timer_Tick), null, 0, 1);

            return "Hello";
        }

        // 受信サーバ
        private string ServerNotificationCommunicator(string command, int row, int col, string value)
        {
            string sendMsg = string.Empty;

            sendMsg = SendMessageCreator(command, row, col, value);

            //NetworkStreamを取得
            //サーバーにデータを送信する
            //文字列をByte型配列に変換
            System.Text.Encoding enc = System.Text.Encoding.UTF8;
            byte[] sendBytes = enc.GetBytes(sendMsg);
            //データを送信する
            ns.Write(sendBytes, 0, sendBytes.Length);
            //Console.WriteLine(sendMsg);

            //Thread.Sleep(1000);

            //サーバーから送られたデータを受信する
            System.IO.MemoryStream ms = new System.IO.MemoryStream();
            byte[] resBytes = new byte[256];
            int resSize = 0;
            do
            {
                //データの一部を受信する
                resSize = ns.Read(resBytes, 0, resBytes.Length);
                //Readが0を返した時はサーバーが切断したと判断
                if (resSize == 0)
                {
                    Console.WriteLine("サーバーが切断しました。");
                    break;
                }
                //受信したデータを蓄積する
                ms.Write(resBytes, 0, resSize);
                //まだ読み取れるデータがあるか、データの最後が\nでない時は、
                // 受信を続ける
            } while (ns.DataAvailable);
            //受信したデータを文字列に変換
            string resMsg = enc.GetString(ms.GetBuffer(), 0, (int)ms.Length);
            ms.Close();
            //末尾の\nを削除
            resMsg = resMsg.TrimEnd('\n');
            //Console.WriteLine(resMsg);

            // {"response":{"row":0,"col":0,"value":"ne"}} が返って来た時に、


            //JsonResponse m = JsonConvert.DeserializeObject<JsonResponse>(resMsg);

            //SetOppositeMove(m.response.row, m.response.col, m.response.value);



            //Json パーサーの移植

            Json json = new Json();

            StringBuilder element = new StringBuilder(1024);

            json.ParseJson(resMsg, out element);

            //MessageBox.Show(json.pair[10].key + json.pair[11].value);
            int l = 0;
            while (l < json.pair.Count)
            {
                var h = json.pair[l].value;
                Console.WriteLine("key = " + json.pair[l].key + " : value = " + h);
                l++;
            }

            foreach(var s in json.pair)
            {
                Console.WriteLine("key = " + s.key);
                Console.WriteLine("value = " + s.value);
            }


            l = 0;
            while (l < json.pair.Count)
            {
                string r = string.Empty;
                string c = string.Empty;
                string v = string.Empty;
                string p = string.Empty;

                if ("row" == json.pair[l].key)
                {
                    r = json.pair[l].value;
                    l++;
                    if ("col" == json.pair[l].key)
                    {
                        c = json.pair[l].value;
                        l++;
                        if ("value" == json.pair[l].key)
                        {
                            v = json.pair[l].value;
                            if ("\"ne\"" == v)
                            {
                                v = "ね";
                            }
                            else
                            {
                                v = "こ";
                            }


                            l++;

                            if ("point" == json.pair[l].key)
                            {
                                myPoint += int.Parse(json.pair[l].value);

                                label2.BeginInvoke(new Action(() => { label2.Text = myPoint.ToString(); }));
                            }

                            l++;

                            if ("response_my" == json.pair[l].key)
                            {
                                //SetOppositeMove(int.Parse(r), int.Parse(c), v);
                            }
                        }
                    }
                }

                l++;
            }

            

            sendMsg = "{\"waiting\":\"OK\"}";

            sendBytes = enc.GetBytes(sendMsg);

            ns.Write(sendBytes, 0, sendBytes.Length);

            ms = new System.IO.MemoryStream();
            resBytes = new byte[256];
            resSize = 0;

            do
            {
                resSize = ns.Read(resBytes, 0, resBytes.Length);

                if (resSize == 0)
                {
                    Console.WriteLine("サーバーが切断しました。");
                    break;
                }

                ms.Write(resBytes, 0, resSize);

            } while (ns.DataAvailable);

            resMsg = enc.GetString(ms.GetBuffer(), 0, (int)ms.Length);
            ms.Close();

            resMsg = resMsg.TrimEnd('\n');

            Console.WriteLine(resMsg);



            //Json パーサーの移植

            json = new Json();

            element = new StringBuilder(1024);

            json.ParseJson(resMsg, out element);

            //MessageBox.Show(json.pair[10].key + json.pair[11].value);
            l = 0;
            while (l < json.pair.Count)
            {
                var h = json.pair[l].value;
                Console.WriteLine("key = " + json.pair[l].key + " : value = " + h);
                l++;
            }

            foreach (var s in json.pair)
            {
                Console.WriteLine("key = " + s.key);
                Console.WriteLine("value = " + s.value);
            }


            l = 0;
            while (l < json.pair.Count)
            {
                string r = string.Empty;
                string c = string.Empty;
                string v = string.Empty;
                string p = string.Empty;

                if ("row" == json.pair[l].key)
                {
                    r = json.pair[l].value;
                    l++;
                    if ("col" == json.pair[l].key)
                    {
                        c = json.pair[l].value;
                        l++;
                        if ("value" == json.pair[l].key)
                        {
                            v = json.pair[l].value;
                            if ("\"ne\"" == v)
                            {
                                v = "ね";
                            }
                            else
                            {
                                v = "こ";
                            }


                            l++;

                            if ("point" == json.pair[l].key)
                            {
                                opPoint += int.Parse(json.pair[l].value);

                                label3.BeginInvoke(new Action(() => { label3.Text = opPoint.ToString(); }));
                            }

                            l++;

                            if ("response_op" == json.pair[l].key)
                            {
                                SetOppositeMove(int.Parse(r), int.Parse(c), v);
                            }
                        }
                    }
                }

                l++;
            }

            if (resMsg == "OK")
            {
                //MessageBox.Show("OK");

                // グローバルの通知専用変数に代入

                timer = new System.Threading.Timer(new TimerCallback(Timer_Tick), null, 0, 1);

                serverNotification = "start";

                return "Hello";

            }
            else if (resMsg == "disconnect")
            {
                //MessageBox.Show("disconnect");

                timer.Change(Timeout.Infinite, Timeout.Infinite);

                CommunicationEnd();

                return "Close";
            }

            

            timer = new System.Threading.Timer(new TimerCallback(Timer_Tick), null, 0, 1);

            return "Hello";
        }

        private void SetDefaultBoard(int row, int col, string value)
        {
            cells[row, col].BeginInvoke(new Action(() => {
                cells[row, col].Text = value;
                cells[row, col].ReadOnly = true;
                cells[row, col].BackColor = Color.LightGray;
            }));
            
        }

        private void SetOppositeMove(int row, int col, string value)
        {
            cells[row, col].BeginInvoke(new Action(() => {
                cells[row, col].Text = value;
                cells[row, col].ReadOnly = true;
                cells[row, col].BackColor = Color.Aqua;
            })); ;
        }
    }
}
