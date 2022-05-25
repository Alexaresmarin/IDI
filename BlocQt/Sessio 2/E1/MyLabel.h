#include <QLabel>
class MyLabel: public QLabel
{
	Q_OBJECT
	public:
		MyLabel (QWidget *parent);
		QString text;
		
	public slots:
		void truncarLabel(int);
		void Text(QString s);

};
