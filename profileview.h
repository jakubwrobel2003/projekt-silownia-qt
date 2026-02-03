



#ifndef PROFILEVIEW_H

#define PROFILEVIEW_H



#include <QWidget>

#include "./Model/usermodel.h"



    namespace Ui {

    class ProfileView;

}



class ProfileView : public QWidget

{

    Q_OBJECT



public:

    explicit ProfileView(QWidget *parent = nullptr);

    ~ProfileView();



    void setUserData(UserModel *currentUser);



private slots:

    void on_btnChangePhoto_clicked();

    void on_btnAddMeasurement_clicked();

    void on_btnCalculateCalories_clicked(); // <--- NOWY SLOT



private:

    Ui::ProfileView *ui;

    UserModel *currentUser = nullptr;



    void loadAvatar();

    void loadMeasurementsHistory();

    QString getBMICategory(double bmi);

    QString getBMIColor(double bmi);



    // Funkcja pomocnicza do inicjalizacji ComboBoxów

    void setupCalorieCalculator();

};



#endif // PROFILEVIEW_H
