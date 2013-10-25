/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package hmm;

/**
 *
 * @author code
 */
public class HMM {

    /**
     * @param args the command line arguments
     */
    public static void main(String[] args) throws Exception {
        // TODO code application logic here
        trainingCorpus tc = new trainingCorpus();
        
        //change the training corpus and output path names accordingly
        tc.produceInput("/home/code/NetBeansProjects/HMM/src/hmm/training_corpus1.txt");
        tc.printToFile("/home/code/NetBeansProjects/HMM/src/hmm/corpus_tables1.txt");
        tc.produceInput("/home/code/NetBeansProjects/HMM/src/hmm/training_corpus2.txt");
        tc.printToFile("/home/code/NetBeansProjects/HMM/src/hmm/corpus_tables2.txt");
        tc.produceInput("/home/code/NetBeansProjects/HMM/src/hmm/training_corpus3.txt");
        tc.printToFile("/home/code/NetBeansProjects/HMM/src/hmm/corpus_tables3.txt");
        tc.produceInput("/home/code/NetBeansProjects/HMM/src/hmm/training_corpus4.txt");
        tc.printToFile("/home/code/NetBeansProjects/HMM/src/hmm/corpus_tables4.txt");
        tc.produceInput("/home/code/NetBeansProjects/HMM/src/hmm/training_corpus5.txt");
        tc.printToFile("/home/code/NetBeansProjects/HMM/src/hmm/corpus_tables5.txt");
    }
}
