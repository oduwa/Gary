/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

package garyeditor;

import java.net.URL;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.Collection;
import java.util.Collections;
import java.util.regex.Matcher;
import java.util.regex.Pattern;
import java.util.stream.Stream;
import javafx.scene.Scene;
import org.fxmisc.richtext.StyleSpans;
import org.fxmisc.richtext.StyleSpansBuilder;

/**
 *
 * @author Odie
 */
public class SyntaxHighlighter {
    
    private static String[] initialiseKeywords(){
        try{
            Path currentRelativePath = Paths.get("");
            String s = currentRelativePath.toAbsolutePath().toString();
            System.out.println(s);
            //URL url = SyntaxHighlighter.class.getResource("/res/keywords.txt");
            Stream<String> stream = Files.lines(Paths.get(s + "/Includes/keywords.kw"));
            
            /* Separate the components of each line */
            String[] lines = (String[]) stream.toArray(String[]::new);
            String[] keywords = new String[lines.length];
            for(int i = 0; i < lines.length; i++){
                keywords[i] = lines[i].split("%")[1];
                System.out.println(keywords[i]);
            }
            
            
            return keywords;
        }
        catch(Exception e){
            e.printStackTrace();
        }
        
        return null;
    }
    
    private static final String[] KEYWORDS = initialiseKeywords();

    private static final String KEYWORD_PATTERN = "\\b(" + String.join("|", KEYWORDS) + ")\\b";
    private static final String PAREN_PATTERN = "\\(|\\)";
    private static final String BRACE_PATTERN = "\\{|\\}";
    private static final String BRACKET_PATTERN = "\\[|\\]";
    private static final String SEMICOLON_PATTERN = "\\;";
    private static final String STRING_PATTERN = "\"([^\"\\\\]|\\\\.)*\"";
    private static final String COMMENT_PATTERN = "#[^\n]*" + "|" + "/\\*(.|\\R)*?\\*/";
    private static final String VARIABLE_PATTERN = "(@)(?:[a-z][a-z]+)";

    private static final Pattern PATTERN = Pattern.compile(
            "(?<KEYWORD>" + KEYWORD_PATTERN + ")"
            + "|(?<PAREN>" + PAREN_PATTERN + ")"
            + "|(?<BRACE>" + BRACE_PATTERN + ")"
            + "|(?<BRACKET>" + BRACKET_PATTERN + ")"
            + "|(?<SEMICOLON>" + SEMICOLON_PATTERN + ")"
            + "|(?<STRING>" + STRING_PATTERN + ")"
            + "|(?<COMMENT>" + COMMENT_PATTERN + ")"
            + "|(?<VARIABLE>" + VARIABLE_PATTERN + ")"
    , Pattern.CASE_INSENSITIVE | Pattern.DOTALL);
    
    public static void applyStyleSheetToScene(Scene scene, String stylesheetLocation){
        scene.getStylesheets().add(GaryEditor.class.getResource(stylesheetLocation).toExternalForm());
    }
    
    public static StyleSpans<Collection<String>> computeHighlighting(String text) {
        Matcher matcher = PATTERN.matcher(text);
        int lastKwEnd = 0;
        StyleSpansBuilder<Collection<String>> spansBuilder
                = new StyleSpansBuilder<>();
        while(matcher.find()) {
            String styleClass =
                    matcher.group("KEYWORD") != null ? "keyword" :
                    matcher.group("PAREN") != null ? "paren" :
                    matcher.group("BRACE") != null ? "brace" :
                    matcher.group("BRACKET") != null ? "bracket" :
                    matcher.group("SEMICOLON") != null ? "semicolon" :
                    matcher.group("STRING") != null ? "string" :
                    matcher.group("COMMENT") != null ? "comment" :
                    matcher.group("VARIABLE") != null ? "variable" :
                    null; /* never happens */ assert styleClass != null;
            spansBuilder.add(Collections.emptyList(), matcher.start() - lastKwEnd);
            spansBuilder.add(Collections.singleton(styleClass), matcher.end() - matcher.start());
            lastKwEnd = matcher.end();
        }
        spansBuilder.add(Collections.emptyList(), text.length() - lastKwEnd);
        return spansBuilder.create();
    }
    
}
