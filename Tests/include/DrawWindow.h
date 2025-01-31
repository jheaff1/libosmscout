/*
  DrawTextQt - a test program for libosmscout
  Copyright (C) 2017  Lukas Karas

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#ifndef TEST_DRAWWINDOW_H
#define TEST_DRAWWINDOW_H

#include <QApplication>
#include <QElapsedTimer>
#include <QPixmap>
#include <QScreen>
#include <QWidget>

#include <osmscout/Database.h>
#include <osmscoutmap/MapService.h>

#include <osmscoutmap/LabelPath.h>

#include <QtGui>

class DrawWindow : public QWidget
{
  Q_OBJECT
public:
  explicit DrawWindow(QString variant, int sinCount, QWidget *parent = 0);

  virtual ~DrawWindow();

protected:
  virtual void paintEvent(QPaintEvent *event);

  void setupTransformation(QPainter *painter, const osmscout::LabelPath &p,
                           const qreal offset, const qreal baseline) const;

  void drawText1(QPainter *painter, QString string, const osmscout::LabelPath &p);
  void drawText2(QPainter *painter, QString string, const osmscout::LabelPath &p);

  void drawLine(QPainter *painter, const osmscout::LabelPath &p);

  QString                   variant;
  std::vector<double>       sin;           //! Lookup table for sin calculation
  int                       sinCount;
  int                       cnt;
  QElapsedTimer             timer;
  QElapsedTimer             animTimer;
  int                       startOffset;
  double                    moveOffset;
};

#endif /* TEST_DRAWWINDOW_H */

