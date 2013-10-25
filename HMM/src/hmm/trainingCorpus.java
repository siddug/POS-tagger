/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package hmm;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.DataInputStream;
import java.io.FileInputStream;
import java.io.FileWriter;
import java.io.InputStreamReader;
import java.util.HashMap;
import java.util.Set;
import java.util.Vector;

/**
 *
 * @author code
 */
public class trainingCorpus {
    
    HashMap transition_count;
    HashMap word_count;
    HashMap initial_pos_count;
    HashMap word_id_map;
    /**
     * @param args the command line arguments
     */
    trainingCorpus(){
        transition_count = new HashMap<String,HashMap<String,Integer>>();
        
        HashMap temp = new HashMap<String,Integer>();
        temp.put("N", 0);temp.put("V", 0);temp.put("A", 0);temp.put("R", 0);temp.put("O", 0);
        transition_count.put("N", temp);
        
        temp = new HashMap<String,Integer>();
        temp.put("N", 0);temp.put("V", 0);temp.put("A", 0);temp.put("R", 0);temp.put("O", 0);
        transition_count.put("V", temp);
        
        temp = new HashMap<String,Integer>();
        temp.put("N", 0);temp.put("V", 0);temp.put("A", 0);temp.put("R", 0);temp.put("O", 0);
        transition_count.put("A", temp);
        
        temp = new HashMap<String,Integer>();
        temp.put("N", 0);temp.put("V", 0);temp.put("A", 0);temp.put("R", 0);temp.put("O", 0);
        transition_count.put("R", temp);
        
        temp = new HashMap<String,Integer>();
        temp.put("N", 0);temp.put("V", 0);temp.put("A", 0);temp.put("R", 0);temp.put("O", 0);
        transition_count.put("O", temp);
        
        word_count = new HashMap<String,HashMap<String,Integer>>();
        
        initial_pos_count = new HashMap<String,Integer>();
        initial_pos_count.put("N", 0);initial_pos_count.put("V", 0);initial_pos_count.put("A", 0);
        initial_pos_count.put("R", 0);initial_pos_count.put("O", 0);
        
        word_id_map = new HashMap<String,Integer>();
        
    }
    
    void produceInput(String in_file) throws Exception{
        
        BufferedReader reader = new BufferedReader(new InputStreamReader(new DataInputStream(new FileInputStream(in_file))));
        String line;
        int id_count=1;
        while((line=reader.readLine())!=null){
            String[] tokens = line.split(" ");
            String prev_pos=null;
            for(int i=0;i<tokens.length;i++){
                String[] partition = tokens[i].split("_");
                if(i==0){
                    prev_pos=partition[1];
                    initial_pos_count.put(partition[1], (Integer)initial_pos_count.get(partition[1])+1);
                }
                else{
                    HashMap cur_trans_count = (HashMap<String,Integer>)transition_count.get(prev_pos);
                    cur_trans_count.put(partition[1], (Integer)cur_trans_count.get(partition[1])+1);
                    prev_pos = partition[1];
                }
                if(word_count.containsKey(partition[0])){
                    HashMap cur_count = (HashMap<String,Integer>)word_count.get(partition[0]);
                    cur_count.put(partition[1], (Integer)cur_count.get(partition[1])+1);
                }
                else{
                    word_id_map.put(partition[0], id_count);
                    id_count++;
                    HashMap initial_word_count = new HashMap<String,Integer>();
                    initial_word_count.put("N", 0);initial_word_count.put("V", 0);initial_word_count.put("A", 0);
                    initial_word_count.put("R", 0);initial_word_count.put("O", 0);
                    initial_word_count.put(partition[1], 1);
                    word_count.put(partition[0],initial_word_count);
                }
            }
        }
        reader.close(); 
    }
    
    void printToFile(String out_file) throws Exception{
        FileWriter fstream = new FileWriter(out_file);
        BufferedWriter out = new BufferedWriter(fstream);
        
        //out.append("total POS tags used: "+transition_count.size()+"\n");
        //out.append("unique words in the corpus: "+word_count.size()+"\n");
        out.append(""+transition_count.size()+"\n");
        out.append(""+word_count.size()+"\n");
        
        Set<String> keys = initial_pos_count.keySet();
        int count=0;
        for(String key:keys){
            count+=(Integer)initial_pos_count.get(key);
        }
        //out.append("Total number of lines in the corpus: "+count+"\n------------------------\n----transition table Columns : V A R N O----\n----transition table rows: ^ V A R N O-----\n");
        for(String key:keys){
            out.append((Integer)initial_pos_count.get(key)/(float)count+" ");
        }
        out.append("\n");
        
        keys=transition_count.keySet();
        for(String key : keys){
            //out.append(key+" | ");
            HashMap<String,Integer> cur_count = (HashMap<String,Integer>)transition_count.get(key);
            Set<String> pos_keys = cur_count.keySet();
            count = 0;
            for(String pos : pos_keys){
                count+=cur_count.get(pos);
            }
            for(String pos : pos_keys){
                out.append(cur_count.get(pos)/(float)count+" ");
            }
            out.append("\n");
        }
        
        //out.append("---------------------\n---------observation probability table-----\n--------columns: word_id word V A R N O----\n");
        keys=word_count.keySet();
        for(String key : keys){
            HashMap<String,Integer> cur_count = (HashMap<String,Integer>)word_count.get(key);
            Set<String> pos_keys = cur_count.keySet();
            out.append(word_id_map.get(key)+" "+key+" ");
            count = 0;
            for(String pos : pos_keys){
                count+=cur_count.get(pos);
            }
            for(String pos : pos_keys){
                out.append((Integer)cur_count.get(pos)/(float)count+" ");
            }
            out.append("\n");
        }
        out.close();
    }
    
    HashMap<String,String> convertSentence(String line){
    /*    String[] words = line.split(" ");
        boolean exists=true;
        for(String word:words){
            if(!word_id_map.containsKey(word)){
                exists=false;
            }
        }
        if(words.length<=1){exists=false;}
        if(exists){
            Vector<String> tag_sequence = null; // VARNO
            Vector<Float> probability = null;
            Set<String> keys = initial_pos_count.keySet();
            int count=0;
            for(String key:keys){
                count+=(Integer)initial_pos_count.get(key);
            }
            Vector<String>tags = null;
            tags.add("V");tags.add("A");tags.add("R");tags.add("N");tags.add("O");
            
            HashMap<String,Integer> cur_count = (HashMap<String,Integer>)word_count.get(words[0]);
            Set<String> pos_keys = cur_count.keySet();
            int count2 = 0;
            for(String pos : pos_keys){
                count2+=cur_count.get(pos);
            }
            for(int i=0;i<tags.size();i++){
                tag_sequence.add(tags.elementAt(i));
                probability.add(((Integer)initial_pos_count.get(tags.elementAt(i))/(float)count)*((Integer)cur_count.get(tags.elementAt(i))/(float)count2));
            }
            
            for(int i=1;i<words.length;i++){
                for(int j=0;j<tags.size();j++){
                    String newtagsequence="";
                    float max_prob=-1;
                    for(int k=0;k<tag_sequence.size();k++){
                        String t=tag_sequence.elementAt(k).substring(tag_sequence.elementAt(k).length()-1,tag_sequence.elementAt(k).length());
                        HashMap<String,Integer> cur_count2 = (HashMap<String,Integer>)transition_count.get(t);
                        Set<String> pos_keys2 = cur_count2.keySet();
                        int count3 = 0;
                        for(String pos : pos_keys2){
                            count3+=cur_count2.get(pos);
                        }
                        float prob=cur_count2.get(tags.elementAt(j))/(float)count3;
                        HashMap<String,Integer> cur_count3 = (HashMap<String,Integer>)word_count.get(words[i]);
                        Set<String> pos_keys3 = cur_count3.keySet();
                        int count4 = 0;
                        for(String pos : pos_keys3){
                            count4+=cur_count3.get(pos);
                        }
                        prob*=((Integer)cur_count3.get(tags.elementAt(j))/(float)count4);
                        if(max_prob==-1||prob>max_prob){
                            max_prob=prob;
                            newtagsequence=tagsequence+
                        }
                    }
                }
            }
            
        }*/
        return null;
    }
    
    /*public static void main(String[] args) throws Exception{
        // TODO code application logic here
        
        if(args[0].equals("build")){
            tc.printToFile(args[2]);
        }
        else if(args[0].equals("convert"))
            tc.convertSentence(args[2]);
        
    }*/
}
