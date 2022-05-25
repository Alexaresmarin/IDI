#include <QLabel>
class MyLabel: public QLabel
{
	Q_OBJECT
	public:
		MyLabel (QWidget *parent);
		int r = 0, g = 0, b = 0;
		
	public slots:
		void R(int n);
		void G(int n);
		void B(int n);
	signals:
		void EnviarRGB(float, float, float);
};
