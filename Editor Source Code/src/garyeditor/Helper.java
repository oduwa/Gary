/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

package garyeditor;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.io.InputStreamReader;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.Optional;
import java.util.logging.Level;
import java.util.logging.Logger;
import javafx.scene.control.TextInputDialog;
import javafx.stage.FileChooser;

/**
 *
 * @author Odie
 */
public class Helper {
    
    public static String getCurrentPath(){
        Path currentRelativePath = Paths.get("");
        String s = currentRelativePath.toAbsolutePath().toString();
        return s;
    }
    
    public static FileChooser getOpenFileChooser(){
        FileChooser fileChooser = new FileChooser();
        fileChooser.setTitle("View Pictures");
        fileChooser.setInitialDirectory(
                new File(System.getProperty("user.dir")/*System.getProperty("user.home")*/)
        );
        fileChooser.getExtensionFilters().addAll(
                new FileChooser.ExtensionFilter("TXT", "*.txt"),
                new FileChooser.ExtensionFilter("KC", "*.kc")
        );
        return fileChooser;
    }
    
    public static String presentSaveFileDialog(String defaultName){
        TextInputDialog dialog = new TextInputDialog(defaultName);
        dialog.setTitle("Save as");
        dialog.setHeaderText("Save your awesome new source code!");
        dialog.setContentText("");
        
        Optional<String> result = dialog.showAndWait();
        if (result.isPresent()) {
            return result.get();
        }
        else{
            return "";
        }
    }
    
    public static void writeToFile(String filename, String content){
        FileWriter writer;
        try {
            writer = new FileWriter(filename, false);
            BufferedWriter bufferedWriter = new BufferedWriter(writer);

            bufferedWriter.write(content);

            bufferedWriter.close();
        } catch (IOException ex) {
            Logger.getLogger(Helper.class.getName()).log(Level.SEVERE, null, ex);
        }
        
    }
    
    public static String readFile(String filename){
        StringBuilder sb = new StringBuilder();
        FileReader reader;
        
        try {
            reader = new FileReader(filename);
            BufferedReader bufferedReader = new BufferedReader(reader);

            String line;

            while ((line = bufferedReader.readLine()) != null) {
                sb.append(line);
                sb.append("\n");
            }
            reader.close();
        } catch (FileNotFoundException ex) {
            Logger.getLogger(Helper.class.getName()).log(Level.SEVERE, null, ex);
        } catch (IOException ex) {
            Logger.getLogger(Helper.class.getName()).log(Level.SEVERE, null, ex);
        }
        
        return sb.toString();
        
    }
    
    public static String executeCommand(String command) {

		StringBuffer output = new StringBuffer();

		Process p;
		try {
			p = Runtime.getRuntime().exec(command);
			p.waitFor();
			BufferedReader reader = 
                            new BufferedReader(new InputStreamReader(p.getInputStream()));

                        String line = "";			
			while ((line = reader.readLine())!= null) {
				output.append(line + "\n");
			}

		} catch (Exception e) {
			e.printStackTrace();
		}

		return output.toString();

	}
    
}
