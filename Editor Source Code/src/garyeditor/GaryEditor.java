/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

package garyeditor;

import java.io.File;
import java.util.Collection;
import java.util.Collections;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import javafx.application.Application;
import javafx.beans.value.ChangeListener;
import javafx.beans.value.ObservableValue;
import javafx.event.ActionEvent;
import javafx.event.EventHandler;
import javafx.scene.Scene;
import javafx.scene.control.Button;
import javafx.scene.control.Menu;
import javafx.scene.control.MenuBar;
import javafx.scene.control.MenuItem;
import javafx.scene.control.TextArea;
import javafx.scene.control.ToolBar;
import javafx.scene.image.ImageView;
import javafx.scene.layout.BorderPane;
import javafx.scene.layout.StackPane;
import javafx.scene.layout.VBox;
import javafx.stage.FileChooser;
import javafx.stage.Stage;


import org.fxmisc.richtext.CodeArea;
import org.fxmisc.richtext.LineNumberFactory;
import org.fxmisc.richtext.StyleSpans;
import org.fxmisc.richtext.StyleSpansBuilder;

/**
 *
 * @author Odie
 */
public class GaryEditor extends Application {
    
    private static CodeArea codeArea;
    private static TextArea consoleArea;
    private static BorderPane root;
    private static Stage stage;
    private static String workingFileLocation;
    
    @Override
    public void start(Stage primaryStage) {
        
        // Create view hierarchy
        root = new BorderPane();
        VBox topContainer = new VBox();  //Creates a container to hold all Menu Objects.
        MenuBar mainMenu = new MenuBar();  //Creates our main menu to hold our Sub-Menus.
        ToolBar toolBar = new ToolBar();  //Creates our tool-bar to hold the buttons.
        
        topContainer.getChildren().add(mainMenu);
        topContainer.getChildren().add(toolBar);
        root.setTop(topContainer);
        
        
        // Menu
        initialiseMenu(mainMenu);
        
        // Toolbar
        initialiseToolbar(toolBar);
        
        // Code Area
        codeArea = createCodeArea();
        root.setCenter(codeArea);
        
        // Console Area
        consoleArea = createConsoleArea();
        root.setBottom(consoleArea);
        
        // Tie it all together
        Scene scene = new Scene(root, 640, 400);
        SyntaxHighlighter.applyStyleSheetToScene(scene, "java-keywords.css");
        
        primaryStage.setTitle("KiddyC Playground");
        primaryStage.setScene(scene);
        primaryStage.show();
        stage = primaryStage;
    }
    
    private static void initialiseMenu(MenuBar mainMenu){
        //Declare sub-menus and add to main menu.
        //Create and add the "File" sub-menu options. 
        Menu file = new Menu("File");
        MenuItem openFile = new MenuItem("Open File");
        MenuItem exitApp = new MenuItem("Exit");
        file.getItems().addAll(openFile, exitApp);

        //Create and add the "Edit" sub-menu options.
        Menu edit = new Menu("Edit");
        MenuItem properties = new MenuItem("Properties");
        edit.getItems().add(properties);

        //Create and add the "Help" sub-menu options.
        Menu help = new Menu("Help");
        MenuItem visitWebsite = new MenuItem("Visit Website");
        help.getItems().add(visitWebsite);
 
        mainMenu.getMenus().addAll(file, edit, help);
    }
    
    private static void saveCode(){
        /* Get input for filename */
        String filename = Helper.presentSaveFileDialog(workingFileLocation);
        if(filename.isEmpty()){
            workingFileLocation = "";
            return;
        }

        /* Add extension if user didnt */
        if (!filename.contains(".txt") && !filename.contains(".kc")) {
            filename = filename.concat(".txt");
        }

        /* Append folder to filename if the user didnt */
        // TODO: potential bug if user simply enters something like"a.txt_My Code/"
        if (!filename.contains("MyCode/")) {
            filename = "MyCode/" + filename;
        }

        /* Save File */
        File dir = new File("MyCode");
        dir.mkdir();
        Helper.writeToFile(filename, codeArea.getText());

        workingFileLocation = filename;
        consoleArea.appendText("Saved file " + filename + "\n");
    }
    
    private static void initialiseToolbar(ToolBar toolBar){
        //Create some Buttons for the Toolbar.
        Button saveBtn = new Button();
        Button runBtn = new Button();
        Button consoleBtn = new Button();
        Button openBtn = new Button();
        
        runBtn.setOnAction(new EventHandler<ActionEvent>() {
            @Override
            public void handle(ActionEvent e) {
                /* Ensure there is something saved to run */
                if(workingFileLocation == null || workingFileLocation.isEmpty()){
                    saveCode();
                }
                System.out.println("x"+workingFileLocation+"x");
                if(workingFileLocation.isEmpty()){
                    consoleArea.appendText("ERROR: NO FILE FOUND. Open or save a file to run\n");
                    return;
                }
                
                
                /* Create copy of filename to run */
                String filename = workingFileLocation;
                if(!filename.contains("MyCode/")){
                    filename = "MyCode/" + filename;
                }
                
                //consoleArea.appendText(Helper.executeCommand("pwd") + "\n");
                String osName = System.getProperty("os.name");
                
                if(osName.startsWith("Windows")){
                    consoleArea.appendText("\nKiddyC.exe \"" + filename + "\"" + "\n\n");
                    consoleArea.appendText((Helper.executeCommand("KiddyC.exe " + filename) + "\n").replace("[35m", "").replace("[0m", ""));
                }
                else{
                    consoleArea.appendText("\n./KiddyC \"" + filename + "\"" + "\n\n");
                    consoleArea.appendText((Helper.executeCommand("./KiddyC " + filename) + "\n").replace("[35m", "").replace("[0m", ""));
                }
                
                
            }
        });
        
        saveBtn.setOnAction(new EventHandler<ActionEvent>() {
            @Override
            public void handle(ActionEvent e) {
               saveCode(); 
            }
        });
        
        consoleBtn.setOnAction(new EventHandler<ActionEvent>() {
            @Override
            public void handle(ActionEvent e) {
                if(root.getBottom() == null){
                    root.setBottom(consoleArea);
                }
                else{
                    root.setBottom(null);
                }
            }
        });
        
        openBtn.setOnAction(new EventHandler<ActionEvent>() {
            @Override
            public void handle(ActionEvent e) {
                
                FileChooser fileChooser = Helper.getOpenFileChooser();
                File file = fileChooser.showOpenDialog(stage);
                if (file != null) {
                    String fileContents = Helper.readFile(file.getAbsolutePath());
                    codeArea.clear();
                    codeArea.appendText(fileContents);
                    workingFileLocation = file.getName();
                    consoleArea.appendText("Opened file " + workingFileLocation + "\n");
                }
                
            }
        });

        //Set the icon/graphic for the ToolBar Buttons.
        ImageView saveImg = new ImageView("res/diskette.png");
        saveImg.setFitHeight(15);saveImg.setFitWidth(15);
        saveBtn.setGraphic(saveImg);
        
        ImageView runImg = new ImageView("res/run.png");
        runImg.setFitHeight(15);runImg.setFitWidth(15);
        runBtn.setGraphic(runImg);
        
        ImageView consoleImg = new ImageView("res/console.png");
        consoleImg.setFitHeight(15);consoleImg.setFitWidth(15);
        consoleBtn.setGraphic(consoleImg);
        
        ImageView openImg = new ImageView("res/open.png");
        openImg.setFitHeight(15);openImg.setFitWidth(15);
        openBtn.setGraphic(openImg);

        //Add the Buttons to the ToolBar.
        toolBar.getItems().addAll(openBtn, saveBtn, runBtn, consoleBtn);
        //toolBar.getItems().addAll(saveBtn,runBtn);
    }
    
    private static CodeArea createCodeArea(){
        CodeArea codeArea = new CodeArea();
        codeArea.setParagraphGraphicFactory(LineNumberFactory.get(codeArea));
        codeArea.richChanges().subscribe(change -> {
            codeArea.setStyleSpans(0, SyntaxHighlighter.computeHighlighting(codeArea.getText()));
        });
        return codeArea;
    }
    
    private static TextArea createConsoleArea(){
        TextArea textArea = new TextArea("Output:\n");
        textArea.setEditable(false);
        textArea.setPrefHeight(100);
        
        /* Make it auto-scroll to bottom when new stuff is added */
        textArea.textProperty().addListener(new ChangeListener<Object>() {
            @Override
            public void changed(ObservableValue<?> observable, Object oldValue,
                    Object newValue) {
                textArea.setScrollTop(Double.MAX_VALUE); //this will scroll to the bottom
                //use Double.MIN_VALUE to scroll to the top
            }
        });
        
        return textArea;
    }
    

    /**
     * The main() method is ignored in correctly deployed JavaFX application.
     * main() serves only as fallback in case the application can not be
     * launched through deployment artifacts, e.g., in IDEs with limited FX
     * support. NetBeans ignores main().
     *
     * @param args the command line arguments
     */
    public static void main(String[] args) {
        launch(args);
    }
    
}
