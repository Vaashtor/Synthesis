#include "Synthesis.h"
#include <queue>

using namespace System;
using namespace System::Windows::Forms;

[STAThreadAttribute]
void WinMain() {
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);
	
	Синтез::Synthesis form;
	Application::Run(% form);
}

System::Void Синтез::Synthesis::button1_Click(System::Object^ sender, System::EventArgs^ e)
{
	if ((textBox6->Text == "") || (textBox7->Text == "")) Default();
	else {
		dt = Convert::ToDouble(textBox6->Text);
		yz = Convert::ToDouble(textBox7->Text);
	}
	if ((textBox1->Text == "") || (textBox2->Text == "") || (textBox3->Text == "") || (textBox4->Text == "") || (textBox5->Text == "")) Star();
	else {
		K = Convert::ToDouble(textBox1->Text);
		T = Convert::ToDouble(textBox2->Text);
		tau = Convert::ToDouble(textBox3->Text);
		Kp = Convert::ToDouble(textBox4->Text);
		Ti = Convert::ToDouble(textBox5->Text);
	}
	time = Convert::ToDouble(textBox8->Text);
	textBox6->Text = Convert::ToString(dt);
	textBox7->Text = Convert::ToString(yz);
	y1 = 0;
	y = 0;
	sxp = 0;
	dy = yz;
	dyp = yz;
	chart1->Series[0]->Points->Clear();
	chart2->Series[0]->Points->Clear();
	chart2->Series[1]->Points->Clear();
	chart2->Series[2]->Points->Clear();
	chart2->Series[3]->Points->Clear();
	progressBar1->Value = 0;
	progressBar1->Maximum = time/dt;
	progressBar1->Step = 1;
	std::queue <double> yr;
	yr.push(0);
	for (t = 2*dt; t <= tau; t += dt) {
		yr.push(0);
	}
	for (t = dt; t <= time; t += dt)
	{
		//Прехеодная характеристика
		y1 = (y1 + dt / T * yz) * (t > tau);

		//Переходный процесс
		x = dy / Ti + Kp * (dy - dyp) / dt;
		sxp += x;
		y = y + (dt / T * sxp);
		yr.push(Convert::ToDouble(y));
		yr.pop();
		dyp = dy;
		dy = yz - yr.front();

		//Построение графиков
		chart1->Series[0]->Points->AddXY(t, y1);
		if (t <= time - tau) chart2->Series[0]->Points->AddXY(t, yr.front());
		progressBar1->Value = t/dt;
		progressBar1->Refresh();
		chart2->Refresh();

		//Проверка на ошибки
		if (abs(yr.front()) >= 1000) {
			MessageBox::Show("Process is out of control","Error",MessageBoxButtons::OK,MessageBoxIcon::Error);
			break;
		}
	}
	//Построение доверительного интервала
	chart2->Series[1]->Points->AddXY(dt, yz);
	chart2->Series[2]->Points->AddXY(dt, 1.025*yz);
	chart2->Series[3]->Points->AddXY(dt, 0.975*yz);
	chart2->Series[1]->Points->AddXY(t, yz);
	chart2->Series[2]->Points->AddXY(t, 1.025*yz);
	chart2->Series[3]->Points->AddXY(t, 0.975*yz);
	return System::Void();
}

System::Void Синтез::Synthesis::ïðîäâèíóòûéÐåæèìToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e)
{
	label8->Visible = ïðîäâèíóòûéÐåæèìToolStripMenuItem->Checked;
	textBox6->Visible = ïðîäâèíóòûéÐåæèìToolStripMenuItem->Checked;
	label9->Visible = ïðîäâèíóòûéÐåæèìToolStripMenuItem->Checked;
	textBox7->Visible = ïðîäâèíóòûéÐåæèìToolStripMenuItem->Checked;
	label2->Visible = ïðîäâèíóòûéÐåæèìToolStripMenuItem->Checked;
	textBox1->Visible = ïðîäâèíóòûéÐåæèìToolStripMenuItem->Checked;
	return System::Void();
}

void Синтез::Synthesis::Default()
{
	dt = 1;
	yz = 1;
	textBox6->Text = Convert::ToString(dt);
	textBox7->Text = Convert::ToString(yz);
	return System::Void();
}

void Синтез::Synthesis::Star()
{
	K = 1;
	T = 28;
	tau = 1;
	Kp = 1;
	Ti = 30;
	textBox1->Text = Convert::ToString(K);
	textBox2->Text = Convert::ToString(T);
	textBox3->Text = Convert::ToString(tau);
	textBox4->Text = Convert::ToString(Kp);
	textBox5->Text = Convert::ToString(Ti);
	return System::Void();
}
