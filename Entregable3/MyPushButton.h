#include <QPushButton>
class MyPushButton: public QPushButton
{
	Q_OBJECT
	public:
		MyPushButton (QWidget *parent);
		
	public slots:
		void tractar_rectangle();
		void canviar_color_rec();
		void tractar_reset();
		void canviar_color_gris();
		
	signals:
		void pinta();
		void set(const int &);
        
	private:
    	static int n;
		static int rec1, rec2, rec3;
};
