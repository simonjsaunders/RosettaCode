import java.io.*;
import java.util.*;

public class NeighbourWords {
    public static void main(String[] args) {
        try {
            int minLength = 9;
            List<String> words = new ArrayList<>();
            try (BufferedReader reader = new BufferedReader(new FileReader("unixdict.txt"))) {
                String line;
                while ((line = reader.readLine()) != null) {
                    if (line.length() >= minLength)
                        words.add(line);
                }
            }
            String previousWord = null;
            int count = 0;
            for (int i = 0, n = words.size(); i + minLength <= n; ++i) {
                StringBuilder sb = new StringBuilder(minLength);
                for (int j = 0; j < minLength; ++j)
                    sb.append(words.get(i + j).charAt(j));
                String word = sb.toString();
                if (word.equals(previousWord))
                    continue;
                if (Collections.binarySearch(words, word) >= 0)
                    System.out.println(String.format("%2d. %s", ++count, word));
                previousWord = word;
            }
        } catch (Exception e)  {
            e.printStackTrace();
        }
    }
}