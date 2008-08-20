#include "PageFormatDlg.h"
#include "FoColorName.h"
#include "SessionManager.h"

//
/*  Save file as PageFormatDlg.cpp  */
/*  incomming class name PageFormatDlg */
//
#include <QCloseEvent>
//

QPointer<PageFormatDlg> PageFormatDlg::_self = 0L;

//


static QRectF Reducez( QRectF rect , const int percentual )
{
    if (percentual < 1) {
    return rect;
    }
    const int leave_wi = rect.width() * percentual / 100;
    const int leave_hi = rect.height() * percentual / 100;
    return QRectF(0,0,rect.width() - leave_wi, rect.height() - leave_hi);
    /* use CenterRectSlaveFromMaster  to fill on rect target */
}



PageFormatDlg* PageFormatDlg::self( QWidget* parent )
{
    
	if ( !_self )
	_self = new PageFormatDlg( parent );
	return _self;
}

PageFormatDlg::PageFormatDlg( QWidget* parent )
	: QDialog( parent )
{
	 setupUi( this );
     doubleSpinBox->setDecimals(3);
     doubleSpinBox_2->setDecimals(3);
    
    doubleSpinBox_23->setReadOnly(true);
    doubleSpinBox_24->setReadOnly(true);
    doubleSpinBox_25->setReadOnly(true);
    doubleSpinBox_26->setReadOnly(true);
    
     unit = "mm";
     allowUpdate = false;
     fopC = new FopColor();
     l_colors = fopC->fopListColor();
     l_colors.sort();
     fillColorBox();   /* and paper names */
     ApiSession *sx = ApiSession::instance();
     fillFormat( sx->CurrentPageFormat()  );
     allowUpdate = true;
     connect(comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(formatPageChange(int)));
     connect(radioButton, SIGNAL(clicked(bool)), this, SLOT(recalcAll()));
     connect(radioButton_2, SIGNAL(clicked(bool)), this, SLOT(recalcAll()));
    
    connect(comboBox_14, SIGNAL(currentIndexChanged (int)), this, SLOT(recalcAll()));
    connect(comboBox_2, SIGNAL(currentIndexChanged (int)), this, SLOT(recalcAll()));
    connect(comboBox_4, SIGNAL(currentIndexChanged (int)), this, SLOT(recalcAll()));
    connect(comboBox_16, SIGNAL(currentIndexChanged (int)), this, SLOT(recalcAll()));
    connect(comboBox_5, SIGNAL(currentIndexChanged (int)), this, SLOT(recalcAll()));
    connect(comboBox_7, SIGNAL(currentIndexChanged (int)), this, SLOT(recalcAll()));
    connect(comboBox_8, SIGNAL(currentIndexChanged (int)), this, SLOT(recalcAll()));
    connect(comboBox_10, SIGNAL(currentIndexChanged (int)), this, SLOT(recalcAll()));
    connect(comboBox_11, SIGNAL(currentIndexChanged (int)), this, SLOT(recalcAll()));
    connect(comboBox_13, SIGNAL(currentIndexChanged (int)), this, SLOT(recalcAll()));
    connect(lineEdit, SIGNAL(editingFinished()), this, SLOT(recalcAll()));
    
    
    
    QList<QDoubleSpinBox*> wnr;
    wnr << doubleSpinBox << doubleSpinBox_2 << doubleSpinBox_3 << doubleSpinBox_5 << doubleSpinBox_4;
    wnr << doubleSpinBox_6 << doubleSpinBox_7 << doubleSpinBox_8 << doubleSpinBox_9 << doubleSpinBox_10 << doubleSpinBox_11;
    wnr << doubleSpinBox_12 << doubleSpinBox_13 << doubleSpinBox_14;
    wnr << doubleSpinBox_15 << doubleSpinBox_16 << doubleSpinBox_17 << doubleSpinBox_18;
    wnr << doubleSpinBox_19 << doubleSpinBox_20 << doubleSpinBox_21 << doubleSpinBox_22;
    wnr << doubleSpinBox_23 << doubleSpinBox_24 << doubleSpinBox_25 << doubleSpinBox_26;
    
    for (int i = 0; i < wnr.size(); ++i) {
        
        wnr[i]->setSuffix(QString(" ") + unit);
        wnr[i]->setDecimals(3);
    connect(wnr[i], SIGNAL(editingFinished()), this, SLOT(recalcAll()));
    }
    
    
}

void PageFormatDlg::recalcAll()
{
    if (!allowUpdate) {
     return;
    }
    bool insertasnewone = false;
    ApiSession *sx = ApiSession::instance();
    qDebug() << "### recalcAll  ";
    if (lineEdit->text().size() > 2 && currentP.name != lineEdit->text()) {
     currentP.name = lineEdit->text();
     insertasnewone = true;
    }
    
    
    currentP.body.margin_left = ToPoint(doubleSpinBox_3->value(),unit); ////left
    currentP.body.margin_right = ToPoint(doubleSpinBox_5->value(),unit); ////right
    currentP.body.margin_top = ToPoint(doubleSpinBox_4->value(),unit); ////top
    currentP.body.margin_bottom = ToPoint(doubleSpinBox_6->value(),unit);  /// bottom
    
    
    currentP.area[0].enable = yesheader->isChecked();
    currentP.area[1].enable = yesheader_2->isChecked();
    currentP.area[2].enable = yesheader_3->isChecked();
    currentP.area[3].enable = yesheader_4->isChecked();
    
    /*
      FoRegion region_before() const { return area[0]; }
    FoRegion region_after() const { return area[1]; }
    FoRegion region_start() const  { return area[2]; }
    FoRegion region_end()  const { return area[3]; }
    
    */
    
    
    if ( yesheader->isChecked() ) {
    /* header before */
    currentP.area[0].margin_left = ToPoint(doubleSpinBox_7->value(),unit); ////left
    currentP.area[0].margin_right = ToPoint(doubleSpinBox_8->value(),unit); ////right
    currentP.area[0].margin_top = ToPoint(doubleSpinBox_9->value(),unit); ////top
    currentP.area[0].margin_bottom = ToPoint(doubleSpinBox_10->value(),unit);  /// bottom
    currentP.area[0].bg = qvariant_cast<QColor>(currentItemData(comboBox_2));
    /* background color */
    currentP.area[0].bog = qvariant_cast<QColor>(currentItemData(comboBox_4));
    /* border color */
    }
    
    if ( yesheader_2->isChecked() ) {
    /* footer after  */
    currentP.area[1].margin_left = ToPoint(doubleSpinBox_11->value(),unit); ////left
    currentP.area[1].margin_right = ToPoint(doubleSpinBox_12->value(),unit); ////right
    currentP.area[1].margin_top = ToPoint(doubleSpinBox_13->value(),unit); ////top
    currentP.area[1].margin_bottom = ToPoint(doubleSpinBox_14->value(),unit);  /// bottom
    currentP.area[1].bg = qvariant_cast<QColor>(currentItemData(comboBox_5));
    /* background color */
    currentP.area[1].bog = qvariant_cast<QColor>(currentItemData(comboBox_7));
    /* border color */
    }
    
    if ( yesheader_3->isChecked() ) {
    /* region_start()   */
    currentP.area[2].margin_left = ToPoint(doubleSpinBox_15->value(),unit); ////left
    currentP.area[2].margin_right = ToPoint(doubleSpinBox_16->value(),unit); ////right
    currentP.area[2].margin_top = ToPoint(doubleSpinBox_17->value(),unit); ////top
    currentP.area[2].margin_bottom = ToPoint(doubleSpinBox_18->value(),unit);  /// bottom
     currentP.area[2].bg = qvariant_cast<QColor>(currentItemData(comboBox_8));
    /* background color */
    currentP.area[2].bog = qvariant_cast<QColor>(currentItemData(comboBox_10));
    /* border color */
    }
    
    if ( yesheader_4->isChecked() ) {
    /* region_end()   */
    currentP.area[3].margin_left = ToPoint(doubleSpinBox_19->value(),unit); ////left
    currentP.area[3].margin_right = ToPoint(doubleSpinBox_20->value(),unit); ////right
    currentP.area[3].margin_top = ToPoint(doubleSpinBox_22->value(),unit); ////top
    currentP.area[3].margin_bottom = ToPoint(doubleSpinBox_23->value(),unit);  /// bottom
     currentP.area[3].bg = qvariant_cast<QColor>(currentItemData(comboBox_11));
    /* background color */
    currentP.area[3].bog = qvariant_cast<QColor>(currentItemData(comboBox_13));
    /* border color */
    }
    
    
    
    
    
    currentP.G_regt = QRectF(0,0,ToPoint(doubleSpinBox->value(),unit),ToPoint(doubleSpinBox_2->value(),unit));
    currentP.RealSize = currentP.G_regt.size();
    
    
    currentP.body.bg = qvariant_cast<QColor>(currentItemData(comboBox_14));
    currentP.body.bog = qvariant_cast<QColor>(currentItemData(comboBox_16));
    
     ////////doubleSpinBox->setMaximum(maxpageavaiablepaper);   /* largo*/
    ////////////doubleSpinBox_2->setMaximum(maxpageavaiablepaper);  /* alto */
    
    
    ////  landscape / portrait  
    if ( radioButton->isChecked () ) {
     /* portrait  */
     currentP.landscape = false;
    } else {
     currentP.landscape = true;
        
    }
    
    sx->SetPageFormat(currentP);
    if (insertasnewone) {
        
            if (!sx->is_OnChain(currentP)) {
             fillFormat( currentP  );
             return;
            }
            
    } else {
        
        bool existd = sx->coreSave(currentP);
        if (!existd) {
           sx->is_OnChain(currentP);
        }
    }
    /* check if exist if not insert on chain */
    DrawPageResult();
}


void PageFormatDlg::insertItemColorBox( const QString txt , const QColor qcol )
{
    const QIcon colorIcon = createColorIcon(qcol);
    comboBox_14->addItem(colorIcon,txt,qcol);
    comboBox_2->addItem(colorIcon,txt,qcol);
    comboBox_4->addItem(colorIcon,txt,qcol);
    comboBox_16->addItem(colorIcon,txt,qcol);
    comboBox_5->addItem(colorIcon,txt,qcol);
    comboBox_7->addItem(colorIcon,txt,qcol);
    comboBox_8->addItem(colorIcon,txt,qcol);
    comboBox_10->addItem(colorIcon,txt,qcol);
    comboBox_11->addItem(colorIcon,txt,qcol);
    comboBox_13->addItem(colorIcon,txt,qcol);
}

void  PageFormatDlg::fillColorBox()
{
    comboBox_14->clear();
    comboBox_2->clear();
    comboBox_4->clear();
    comboBox_16->clear();
    comboBox_5->clear();
    comboBox_7->clear();
    comboBox_8->clear();
    comboBox_10->clear();
    comboBox_11->clear();
    comboBox_13->clear();
    insertItemColorBox(tr("None Transparent"),QColor(Qt::white));
    for (int i = 0; i < l_colors.size(); ++i)  {
        const QString col = l_colors.at(i);
        QColor current(fopC->foColor(col));
        insertItemColorBox(col,current);
    }
}




void PageFormatDlg::formatPageChange( const int index )
{
   if (!allowUpdate) {
    return;
   }
   allowUpdate = false;
   const int PaperNr = comboBox->itemData(index).toInt();
   ///////qDebug() << "### PaperSwap  " << index;
   ApiSession *sx = ApiSession::instance();
   QMap<int,M_PageSize> allpaper = sx->mpages();
   M_PageSize OtherFormat = allpaper[PaperNr];
   allowUpdate = true;
   fillFormat(OtherFormat);
   qDebug() << "### PaperSwap  " << OtherFormat.name;
}



/*


connect(comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(formatPageChange(int)));


void PageFormatDlg::Load_Connector()
{

  for (int i = 0; i < list.size(); ++i) {   
    std::cout << "### ....." << qPrintable(list.at(i)) << "....." << std::endl; 
     }
     

     
     
 #if defined Q_WS_MAC
 #endif
 #if defined Q_WS_WIN
 #endif
 #if defined Q_WS_X11
 #endif
}

void PageFormatDlg::closeEvent( QCloseEvent* e )
{
	e->accept();
}

*/


/* do nothing */
/*connect(buttonx, SIGNAL(clicked()), this , SLOT(Make()));*/
 /* qDebug() << "#### localedirfile ->" << list.size();   */
/* Q_ASSERT ( bool test )  */ 

/* QString one;  toLower() toUpper()  split(",")  endsWith() startsWith()  QByteArray QString::toAscii() */
/* int nummer; */ 
/* QStringList list;  join();  */ 
/* QUrl url; */ 
/* QByteArray byteArray; */ 
/* bool having; */ 
/* QDomDocument doc;*/ 
/* QDomElement root;*/ 





void  PageFormatDlg::fillFormat( M_PageSize e  )
{
     allowUpdate = false;
     comboBox->clear();
     ApiSession *sx = ApiSession::instance();
     SessionBigerPaper = sx->maximumPage();
    
     lineEdit->setText( e.name );
    
    
    
    
     ///////M_PageSize fpage = sx->CurrentPageFormat();  /* user define or other */
     const QByteArray currentname = e.hashmodel();
     int posi = -1;
     int activeindex = 0;
     QMapIterator<int,M_PageSize> i(sx->mpages());
         while (i.hasNext()) {
             i.next();
             posi++;
             M_PageSize current = i.value();
             if (currentname == current.hashmodel()) {
              activeindex = posi;
             }
             comboBox->addItem(QIcon(":/img/page.png"),current.HName(),i.key());
         }
     comboBox->setCurrentIndex ( activeindex );
     const QRectF rect = e.pageBoundingRect();
         
     /* check area to allow or not */
    bool  headera = e.region_before().enable;
    bool  hfoota = e.region_after().enable;
    bool  hstarta = e.region_start().enable;
    bool  henda = e.region_end().enable;
         
     /*    
          qDebug() << "### 0 "  << headera;
         qDebug() << "### 1 "  << hfoota;
         qDebug() << "### 2 "  << hstarta;
         qDebug() << "### 3 "  <<  henda;
    
      FoRegion region_before() const { return area[0]; }
    FoRegion region_after() const { return area[1]; }
    FoRegion region_start() const  { return area[2]; }
    FoRegion region_end()  const { return area[3]; }
    
    */
         yesheader->setChecked ( headera );
         yesheader_2->setChecked ( hfoota );
         yesheader_3->setChecked ( hstarta );
         yesheader_4->setChecked ( henda );
         
         
         groupBox_3->setEnabled(headera);  /* header */
         groupBox_4->setEnabled(headera);  /* header */
         
         groupBox_5->setEnabled(hfoota);  /* footer */
         groupBox_6->setEnabled(hfoota);  /* footer */
         
         groupBox_7->setEnabled(hstarta);  /* start , left */
         groupBox_8->setEnabled(hstarta);  /* start , left */
         
         groupBox_9->setEnabled(henda);  /* end , right */
         groupBox_10->setEnabled(henda);  /* end , right */
         


    if ( e.region_before().enable ) {
    /* header before */
    ////currentP.area[0].bg = qvariant_cast<QColor>(currentItemData(comboBox_2));
    /////currentP.area[0].bog = qvariant_cast<QColor>(currentItemData(comboBox_4));
    doubleSpinBox_7->setValue ( ToUnit(currentP.region_before().margin_left,unit) ); ////left
    doubleSpinBox_8->setValue ( ToUnit(currentP.region_before().margin_right,unit) ); ////right
    doubleSpinBox_9->setValue ( ToUnit(currentP.region_before().margin_top,unit) ); ////top
    doubleSpinBox_10->setValue ( ToUnit(currentP.region_before().margin_bottom,unit) );  /// bottom
    }
    
    if ( e.region_after().enable ) {
    /* footer after  */
    doubleSpinBox_11->setValue ( ToUnit(currentP.region_after().margin_left,unit) ); ////left
    doubleSpinBox_12->setValue ( ToUnit(currentP.region_after().margin_right,unit) ); ////right
    doubleSpinBox_13->setValue ( ToUnit(currentP.region_after().margin_top,unit) ); ////top
    doubleSpinBox_14->setValue ( ToUnit(currentP.region_after().margin_bottom,unit) );  /// bottom
    ///////currentP.area[1].bg = qvariant_cast<QColor>(currentItemData(comboBox_5));
    //////currentP.area[1].bog = qvariant_cast<QColor>(currentItemData(comboBox_7));
    }
    
    if ( e.region_start().enable ) {
    /* region_start()   */
    doubleSpinBox_15->setValue ( ToUnit(currentP.region_start().margin_left,unit) ); ////left
    doubleSpinBox_16->setValue ( ToUnit(currentP.region_start().margin_right,unit) ); ////right
    doubleSpinBox_17->setValue ( ToUnit(currentP.region_start().margin_top,unit) ); ////top
    doubleSpinBox_18->setValue ( ToUnit(currentP.region_start().margin_bottom,unit) );  /// bottom
    /////currentP.area[2].bg = qvariant_cast<QColor>(currentItemData(comboBox_8));
    //////currentP.area[2].bog = qvariant_cast<QColor>(currentItemData(comboBox_10));
    }
    
    if ( e.region_end().enable ) {
    /* region_end()   */
    doubleSpinBox_19->setValue ( ToUnit(currentP.region_end().margin_left,unit) ); ////left
    doubleSpinBox_20->setValue ( ToUnit(currentP.region_end().margin_right,unit) ); ////right
    doubleSpinBox_21->setValue ( ToUnit(currentP.region_end().margin_top,unit) ); ////top
    doubleSpinBox_22->setValue ( ToUnit(currentP.region_end().margin_bottom,unit) );  /// bottom
    //////currentP.area[3].bg = qvariant_cast<QColor>(currentItemData(comboBox_11));
    ////currentP.area[3].bog = qvariant_cast<QColor>(currentItemData(comboBox_13));
    }
         
         
         
         
         
         
         
         
         
         
         
         
         
         
         
     /* begin to refill all param from e */
     /* unit mm pt in is unit */
         
         ////qreal ToUnit(,unit);
         
         
    /////// QDoubleSpinBox *doubleSpinBox_3; ////left
    ///////QDoubleSpinBox *doubleSpinBox_5; ////right
    ///////QDoubleSpinBox *doubleSpinBox_4; ////top
    ////////QDoubleSpinBox *doubleSpinBox_6;  /// bottom
    ///// doubleSpinBox /// largo
    ///// doubleSpinBox_2 // alto
         
    /* bage index general */
    qreal maxpageavaiablepaper = sx->maximumPage();
    qreal PageLargo = ToUnit(rect.width(),unit);
    const qreal marginmax = PageLargo / 3;
         
    doubleSpinBox_3->setValue ( ToUnit(e.body.margin_left,unit) ); ////left
    doubleSpinBox_5->setValue ( ToUnit(e.body.margin_right,unit) ); ////right
    doubleSpinBox_4->setValue ( ToUnit(e.body.margin_top,unit) ); ////top
    doubleSpinBox_6->setValue ( ToUnit(e.body.margin_bottom,unit) );  /// bottom
    
    
    doubleSpinBox_23->setValue ( ToUnit(e.body.margin_left,unit) ); ////left
    doubleSpinBox_24->setValue ( ToUnit(e.body.margin_right,unit) ); ////right
    doubleSpinBox_25->setValue ( ToUnit(e.body.margin_top,unit) ); ////top
    doubleSpinBox_26->setValue ( ToUnit(e.body.margin_bottom,unit) );  /// bottom
    
    doubleSpinBox_3->setMaximum(marginmax);
    doubleSpinBox_5->setMaximum(marginmax);
    doubleSpinBox_4->setMaximum(marginmax);
    doubleSpinBox_6->setMaximum(marginmax);
    
    doubleSpinBox_23->setMaximum(marginmax);
    doubleSpinBox_24->setMaximum(marginmax);
    doubleSpinBox_25->setMaximum(marginmax);
    doubleSpinBox_26->setMaximum(marginmax);
    
    
         
    doubleSpinBox->setValue (  PageLargo );  /// largo
    doubleSpinBox_2->setValue ( ToUnit(rect.height(),unit) );  /// alto
    
    doubleSpinBox->setMaximum(maxpageavaiablepaper);   /* largo*/
    doubleSpinBox_2->setMaximum(maxpageavaiablepaper);  /* alto */
    
      

    
    
    
    
    if (e.landscape) {
    radioButton->setChecked(false );
    radioButton_2->setChecked(true );
    } else {
    radioButton->setChecked(true );
    radioButton_2->setChecked(false );
    }
    
    /* bage index general */
    sx->SetPageFormat(e);
    currentP = e;
    DrawPageResult();
    
    /* incomming data from */
         qDebug() << "### incomming name  "  << e.name;
         qDebug() << "### 0 "  << headera;
         qDebug() << "### 1 "  << hfoota;
         qDebug() << "### 2 "  << hstarta;
         qDebug() << "### 3 "  <<  henda;
    
    allowUpdate = true;
}



void PageFormatDlg::DrawPageResult()
{
    //// M_PageSize currentP 
    qreal quadra = 1000 * 100;
    QList<QLabel*> wlist;
    wlist << preview << preview_6 << preview_2 << preview_3 << preview_4 << preview_5;
    for (int i = 0; i < wlist.size(); ++i) {
          qreal smallpart = qMin(wlist[i]->height(),wlist[i]->width());
          quadra = qMin(quadra,smallpart);
    }
    const qreal scalerguiyx = SessionBigerPaper / quadra;
    qDebug() << "### scalerguiyx " << scalerguiyx;
    const qreal pagermin = qMin(currentP.pageBoundingRect().height(),currentP.pageBoundingRect().width());
    const qreal scalexy = 0.21;
    QMatrix matrix;
    matrix.scale (scalexy,scalexy);
    
    QRectF inpaperwi =  matrix.mapRect(currentP.pageBoundingRect());
    QRectF margip = matrix.mapRect(QRectF(currentP.body.margin_left,currentP.body.margin_top,currentP.width() - 0.5,currentP.height() - 0.5));
    
    const QRectF PageRect(0,0, quadra, quadra );
    
    QPixmap img(PageRect.size().toSize());
    img.fill(Qt::transparent);
    
    /* image as label space */
    QPainter *painter = new QPainter(&img);
    painter->setRenderHint(QPainter::TextAntialiasing);
    painter->setPen(QPen(Qt::red,0.4));
    painter->setOpacity ( 0.4);
    painter->setBrush(Qt::NoBrush);
    painter->drawRect(PageRect);
    painter->setOpacity ( 1.);
    
    painter->setPen(QPen(Qt::red,0.5));
    QLineF diagonal(PageRect.topLeft(),PageRect.bottomRight());
    QLineF diagonal1(PageRect.topRight(),PageRect.bottomLeft());
    painter->drawLine(diagonal);
    painter->drawLine(diagonal1);
    
    /* image as label space */
    
    /*   papers */
    
    QRectF PageIconRect = CenterRectSlaveFromMaster(PageRect,inpaperwi);
    
    margip.moveTopLeft ( PageIconRect.topLeft() +  margip.topLeft() );
    /* full page */
    painter->setBrush(QColor(Qt::white));
    painter->setPen(QPen(Qt::black,0.3));
    painter->drawRect(PageIconRect);
    /* papers */
    
    
    
    QColor border_color_set =  currentP.body.bog;
    
    /* internal margin  page */
    painter->setBrush(currentP.body.bg);
    if (border_color_set.name() == QColor(Qt::white).name()) {
    painter->setPen(QPen(Qt::black,0.3));
    } else {
    painter->setPen(QPen(border_color_set,0.3));
    }        
    
    
    
    painter->drawRect(margip);
    
    
    
    
    
    
    
    
    
    
    painter->end();
    
    
    
        for (int i = 0; i < wlist.size(); ++i) {
              wlist[i]->setScaledContents ( false );
              wlist[i]->setPixmap(img);
              wlist[i]->setAlignment ( Qt::AlignHCenter | Qt::AlignVCenter );
              ///////wlist[i]->setScaledContents ( true );
         }
    
}


void PageFormatDlg::resizeEvent ( QResizeEvent * event )  
{
    DrawPageResult();
    QDialog::resizeEvent(event);
}





QVariant PageFormatDlg::currentItemData(QComboBox *Box)
 {
     return Box->itemData(Box->currentIndex());
 }











