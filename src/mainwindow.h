#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLocale>
#include <QTranslator>
#include <QLibraryInfo>
#include <QLabel>

#include "about.h"
#include "board.h"

#include "productView.h"
#include "customerView.h"

#include "dbase.h"
#include "settings.h"

namespace Ui {
	class MainWindow;
}

class MainWindow : public QMainWindow {
	Q_OBJECT
public:
	MainWindow(QWidget *parent = 0);
	~MainWindow();
	void init();

protected:
	void changeEvent(QEvent *e);

private:
	Ui::MainWindow *ui;
	board *m_board;
	customerView *m_customerView;
	productView *m_productView;
	about *m_about;
	Settings *m_Settings;
	database *m_database;
	QLocale m_lang;
	QLabel *mlabel_warningBdd;

private slots:
	void on_actionConfiguration_triggered();
	void on_actionA_propos_triggered();
	void on_actionProduits_triggered();
	void on_actionClients_triggered();
	void on_actionTableau_de_bord_triggered();
	void on_actionQuitter_triggered();
	void on_actionA_propos_de_Qt_triggered();

	void on_actionActionProvider_triggered();
	void on_actionLivres_des_Recettes_triggered();
	void on_actionSauvegarder_la_base_de_donn_es_sous_triggered();
	void on_actionServices_common_triggered();
	
	void on_actionOuvrir_BddDir_triggered();
	void on_action_triggered();
	void on_actionClients_ex_triggered();
	void on_actionProduits_ex_triggered();
	void on_actionServices_ex_triggered();
	void on_actionFournisseur_ex_triggered();
	void on_actionSignaler_un_bug_triggered();
	void on_actionImprimer_une_fiche_Ent_te_triggered();

	void on_actionClients_import_triggered();

	void on_actionProduits_import_triggered();

	public slots:
		void RefreshLists();
		void RefreshActivation();
};

#endif // MAINWINDOW_H
