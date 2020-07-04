#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QLabel>
#include <QGraphicsTextItem>
#include <string>
#include <QtMath>
#include <QFileDialog>
#include <QMessageBox>
#include <QDir>
#include <iostream>
#include <fstream>

using namespace std;

QBrush whiteBrush(Qt::white);
QPen blackPen(Qt::black);
QString str;
int i = 0;
int j = 0;




MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    scene = new QGraphicsScene(this);
    ui->setupUi(this);
    ui->graphicsView->setScene(scene);
    blackPen.setWidth(3);
    QFont font;
    font.setPixelSize(12);
    font.setBold(false);
    font.setFamily("Calibri");
}

MainWindow::~MainWindow()
{
    delete ui;

}

void MainWindow::on_pushButton_clicked()
{
    //scene->clear(); scene->addEllipse(10, 10, 30, 30, blackPen, whiteBrush);

    i = 0;
    j = 0;
    scene->clear();
    bool isSymmetric = true;
    QString file_name = QFileDialog::getOpenFileName(this,"Open a file", QDir::homePath());
    QFile file(file_name);
        if(file.open(QIODevice::ReadOnly |QIODevice::Text))
        {
            //Считываем первую строку, чтобы разобраться с количеством вершин
            str = file.readLine();
            int nodesCount = str.size()-1; //Количество вершин графа
            QGraphicsEllipseItem *nodes[nodesCount]; //Создаем массив объектов
            QGraphicsTextItem *texts [nodesCount]; //Создаем массив "подписей" к объектам
            double degrees = 360/nodesCount; //Определяем шаг для хода по огружности
            double radians = qDegreesToRadians(degrees); //Переводим в радианы
            int x[nodesCount]; //Массив для X координат
            int y[nodesCount]; //Массив для Y координат

            //Заполним данные массивы//
            while (i<nodesCount)
            {
                x[i] = nodesCount*15*cos(radians*(i+1));
                y[i] = nodesCount*15*sin(radians*(i+1));
                i++;
            }
            //----------------------//


            //Заполним матрицу смежности//
            j = 0;
            bool lineMatrix[nodesCount][nodesCount];
            for (i = 0; i < nodesCount; i++) lineMatrix[j][i] = strToBool(str.at(i)); //Записали первую строку
            while(!file.atEnd())
            {
                str = file.readLine();
                j++;
                for (i = 0; i < nodesCount; i++) lineMatrix[j][i] = strToBool(str.at(i)); //Записываем строку в нашу матрицу

            }
            file.close();

            for (i = 0; i < nodesCount; i++)
                for (j =0; j < nodesCount; j++)
                   if (lineMatrix[i][j] != lineMatrix[j][i]) isSymmetric = false;
            //Теперь у нас есть матрица смежности

            if (isSymmetric == true)
            {
                ui->graphicsView->setScene(scene);

                //Здесь можно рисовать линии//
                for (i = 0; i < nodesCount; i++)
                    for (j = 0; j < nodesCount; j++)
                        if (lineMatrix[i][j] == true)
                            //обдумать этот момент
                            if (i !=j) scene->addLine(x[i]+15,y[i]+15,x[j]+15,y[j]+15,blackPen);
                            else {
                                if (i < nodesCount/2) scene->addEllipse(x[i]+10, y[i]+20, 15, 15, blackPen, whiteBrush);
                                if (i > nodesCount/2) scene->addEllipse(x[i]+20, y[i]+10, 15, 15, blackPen, whiteBrush);
                                if (i == nodesCount/2) scene->addEllipse(x[i]-5, y[i]+10, 15, 15, blackPen, whiteBrush);
                                if (i == nodesCount) scene->addEllipse(x[i]+10, y[i], 15, 15, blackPen, whiteBrush);
                            }
                //--------------------------//
                i = 0;

                //Теперь рисуем вершины графа
                while (i<nodesCount)
                {
                    nodes[i] = scene->addEllipse(x[i], y[i], 30, 30, blackPen, whiteBrush);
                    QString s = QString::number(i+1);
                    texts[i] = scene->addText(s);
                    texts[i]->setPos(x[i]+5, y[i]+2);
                    i++;
                }
            }
            else
            {
                QMessageBox::information(this, "..", "Введенная Вами матрица не симметрична относительно главной диагонали, а значить по ней нельзя построить неориентированный граф");
            }
            //ui->graphicsView->setScene(scene);
            nodes[1] = scene->addEllipse(x[i], y[i], 30, 30, blackPen, whiteBrush);
        }
}
