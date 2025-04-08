package cli_drivers;

import java.io.File;
import java.util.Locale;
import java.util.Scanner;

//TODO I DONT ACTUALLY THINK THIS IS HOW I WANT TO DO THIS, FIX IT AT SOME POINT
public class ConfigReader {

	public static final String PATH_TO_CONFIG_FILE = "../../../../.config";
	private Scanner configScanner;
	
	//config options 
	private String PATH_TO_FFMPEG;
	
	private ConfigReader(File config_file) {
		configScanner = new Scanner(PATH_TO_CONFIG_FILE);
		parseFile();
	}

	
	public static ConfigReader createConfigReader() throws Exception {
		File f = new File(PATH_TO_CONFIG_FILE);
		if(!f.isFile()) {
			System.err.println("config file not found, try a fresh installation");
			throw new Exception();
		}
		else return new ConfigReader(f);
	}
	
	 private void parseFile() {
		 configScanner.use;
	 }

}
