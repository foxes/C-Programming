
import java.awt.Color;
import java.awt.Font;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.Random;

import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JTextField;
/**
 * Game.
 */
public class WhackAMole implements ActionListener {
    /**
     * every mole has its own button and thread.
     */
    private JButton[] mole;
    /**
     * press to start.
     */
    private JButton startButton;
    /**
     * use JTextField to display state.
     */
    private JTextField timeLeftField, scoreField;
    /**
     * initialize score and state (if end = 0 that means game is not over).
     */
    private int score = 0;
    /**
     * flag to determine if game ends.
     */
    private boolean end = false;
    /**
     * Constructor.
     */
    public WhackAMole() {
        JFrame frame = new JFrame("Whack-a-mole");
        frame.setSize(420, 380);
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        Font font = new Font(Font.MONOSPACED, Font.BOLD, 14);
        JPanel pane = new JPanel();
        startButton = new JButton("Start");
        startButton.addActionListener(this);
        startButton.setFont(font);
        startButton.setOpaque(true);
        pane.add(startButton);
        JLabel timeLeft = new JLabel("Time Left: ");
        pane.add(timeLeft);
        timeLeftField = new JTextField(8);
        pane.add(timeLeftField);
        JLabel scoreLabel = new JLabel("Score: ");
        pane.add(scoreLabel);
        scoreField = new JTextField(8);
        scoreField.setText("" + score);
        pane.add(scoreField);
        mole = new JButton[40];
        for (int i = 0; i < mole.length; i++) {
            mole[i] = new JButton("     ");
            mole[i].setFont(font);
            mole[i].setBackground(Color.LIGHT_GRAY);
            mole[i].setOpaque(true);
            mole[i].addActionListener(this);
            pane.add(mole[i]);
        }
        frame.setContentPane(pane);
        frame.setVisible(true);
    }
    /**
     * main routine.
     * @param args
     * args doesn't matter.
     */
    public static void main(String[] args) {
        new WhackAMole();
    }
    /**
     * in this case, there are just startButton and moles.
     * @param e
     * ActionEvent.
     */
    public void actionPerformed(ActionEvent e) {
        if (e.getSource() == startButton) {
            startButton.setEnabled(false);
            Thread timer = new TimerThread(timeLeftField, startButton);
            timer.start();
            for (int i = 0; i < mole.length; i++) {
                Thread moles = new HelperThread(mole[i]);
                moles.start();
            }
        } else {
            for (int i = 0; i < mole.length; i++) {
                if (e.getSource() == mole[i] && mole[i].getText().equals("HitMe") && end == false) {
                    mole[i].setText("Ouch!");
                    mole[i].setBackground(Color.PINK);
                    score++;
                    if (!timeLeftField.getText().equals("0")) {
                        scoreField.setText("" + score);
                    }
                }
            }
        }
    }
    /**
     * TimerThread.
     */
    private class TimerThread extends Thread {
        /**
         * displaying the left time.
         */
        private JTextField myTimeLeftField;
        /**
         * to start a game.
         */
        private JButton myStartButton;
        /**
         * set game time.
         */
        private int time = 20;
        /**
         * @param timeLeftField
         * displaying the left time.
         * @param startButton
         * start a game.
         */
        public TimerThread(JTextField timeLeftField, JButton startButton) {
            myTimeLeftField = timeLeftField;
            myStartButton = startButton;
        }
        /**
         * run a timer.
         */
        public void run() {
            end = false;
            while (time >= 0) {
                try {
                    myTimeLeftField.setText("" + time);
                    Thread.sleep(1000);
                    time--;
                } catch (InterruptedException e) {
                    // Should not happen
                    e.printStackTrace();
                }
            }
            try {
                end = true;
                score = 0;
                Thread.sleep(5000);
            } catch (InterruptedException e) {
                // Should not happen
                e.printStackTrace();
            }
            myStartButton.setEnabled(true);
        }
    }
    /**
     * HelperThread for every button representing a "mole".
     */
    private class HelperThread extends Thread {
        /**
         * JButton of this thread.
         */
        private JButton mymole;
        /**
         * random number.
         */
        private Random random = new Random();
        /**
         * time while mole is down.
         */
        private int myDownTime = random.nextInt(10) + 2;
        /**
         * time while mole is up.
         */
        private int myUpTime = random.nextInt(3) + 1;
        /**
         * @param mole
         * a mole.
         */
        public HelperThread(JButton mole) {
            mymole = mole;
        }
        /**
         * run a mole hole.
         */
        public void run() {
            mymole.setText("     ");
            mymole.setBackground(Color.LIGHT_GRAY);
            try {
                Thread.sleep(myDownTime * 1000);
            } catch (InterruptedException e1) {
                e1.printStackTrace();
            }
            while (!end) {
                try {
                    mymole.setText("HitMe");
                    mymole.setBackground(Color.green);
                    Thread.sleep(myUpTime * 1000);
                    mymole.setText("     ");
                    mymole.setBackground(Color.LIGHT_GRAY);
                    Thread.sleep(myDownTime * 1000);
                    mymole.setText("     ");
                    mymole.setBackground(Color.LIGHT_GRAY);
                } catch (InterruptedException e) {
                }
            }
        }
    }
}
