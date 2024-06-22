package pacman.display;

import static org.junit.Assert.*;

import javafx.beans.property.SimpleStringProperty;
import javafx.beans.property.StringProperty;
import javafx.collections.FXCollections;
import javafx.collections.ObservableList;
import org.junit.Test;
import pacman.board.BoardItem;
import pacman.board.PacmanBoard;
import pacman.game.PacmanGame;
import pacman.hunter.Speedy;
import pacman.util.Position;

import java.util.ArrayList;
import java.util.List;

/**
 * ScoreViewModelTest
 *
 * Test the ScoreViewModel class
 */
public class ScoreViewModelTest {

    // Create a BoardViewModel test
    private PacmanGame pacmanGameTest = new PacmanGame("Pacman",
            "Usagi Tsukino", new Speedy(),
            new PacmanBoard(20,20));
    private ScoreViewModel scoreViewTest = new ScoreViewModel(pacmanGameTest);


    /**
     * Test for the update() function. This will check that update() is
     * updating correctly if nothing has happened yet.
     */
    @Test
    public void updateDefault() {
        // Check that it is updating the correct things when nothing is changed
        this.scoreViewTest.update();
        StringProperty testCurrentScore = new SimpleStringProperty();
        testCurrentScore.set("Score: 0");
        StringProperty testSortTitle = new SimpleStringProperty();
        testSortTitle.set("Sorted by Name");
        ObservableList<String> scoreListTest = FXCollections
                .observableArrayList();

        // check that these test are equal to what should be the output
        assertEquals(testCurrentScore.toString(),
                this.scoreViewTest.getCurrentScoreProperty().toString());
        assertEquals(testSortTitle.toString(),
                this.scoreViewTest.getSortedBy().toString());
        assertEquals(scoreListTest, this.scoreViewTest.getScores());

    }

    /**
     * Test for the update() function. This will check that update() is
     * updating correctly if something changes.
     */
    @Test
    public void updateAdvanced() {
        // Change current score and add scores to the score board.
        this.pacmanGameTest.getScores().increaseScore(50);
        this.pacmanGameTest.getScores().setScore("Usagi", 200);
        this.pacmanGameTest.getScores().setScore("rei", 50);
        this.pacmanGameTest.getScores().setScore("1235", 60);
        this.scoreViewTest.update();

        // Create the correct solutions for what should be changed
        StringProperty testCurrentScore = new SimpleStringProperty();
        testCurrentScore.set("Score: 50");
        StringProperty testSortTitle = new SimpleStringProperty();
        testSortTitle.set("Sorted by Name");
        ObservableList<String> scoreListTest = FXCollections
                .observableArrayList();
        scoreListTest.setAll("1235 : 60", "Usagi : 200", "rei : 50");

        // Check that it is equal
        assertEquals(testCurrentScore.toString(),
                this.scoreViewTest.getCurrentScoreProperty().toString());
        assertEquals(testSortTitle.toString(),
                this.scoreViewTest.getSortedBy().toString());
        assertEquals(scoreListTest, this.scoreViewTest.getScores());

        // Let's change the sort order and update
        this.scoreViewTest.switchScoreOrder();
        this.scoreViewTest.update();

        // Let's create the correct solutions
        testSortTitle.set("Sorted by Score");
        scoreListTest.setAll("Usagi : 200", "1235 : 60", "rei : 50");

        // Check that it is equal
        assertEquals(testSortTitle.toString(),
                this.scoreViewTest.getSortedBy().toString());
        assertEquals(scoreListTest, this.scoreViewTest.getScores());
    }

    /**
     * Test for the switchScoreOrder() function. This will check that it
     * switches the order of the scores.
     */
    @Test
    public void switchScoreOrder() {
        // Check that it is set to Sorted by Name by default
        StringProperty testSortTitle = new SimpleStringProperty();
        testSortTitle.set("Sorted by Name");
        assertEquals(testSortTitle.toString(),
                this.scoreViewTest.getSortedBy().toString());

        // Flip it to by sorted by score
        this.scoreViewTest.switchScoreOrder();
        this.scoreViewTest.update();
        testSortTitle.set("Sorted by Score");
        assertEquals(testSortTitle.toString(),
                this.scoreViewTest.getSortedBy().toString());

        // Flip it back to being sorted by name
        this.scoreViewTest.switchScoreOrder();
        this.scoreViewTest.update();
        testSortTitle.set("Sorted by Name");
        assertEquals(testSortTitle.toString(),
                this.scoreViewTest.getSortedBy().toString());
    }

    /**
     * Test for the getCurrentScoreProperty() function. This will check that it
     * returns the correct current score in the correct format.
     */
    @Test
    public void getCurrentScoreProperty() {
        // Check that it is returning the current score of 0
        StringProperty testCurrentScore = new SimpleStringProperty();
        testCurrentScore.set("Score: 0");
        assertEquals(testCurrentScore.toString(),
                this.scoreViewTest.getCurrentScoreProperty().toString());

        // Change the score and check it still matches
        testCurrentScore.set("Score: 29");
        this.pacmanGameTest.getScores().increaseScore(29);
        this.scoreViewTest.update();
        assertEquals(testCurrentScore.toString(),
                this.scoreViewTest.getCurrentScoreProperty().toString());

        // Check that it is resetting to 0
        this.pacmanGameTest.getScores().reset();
        testCurrentScore.set("Score: 0");
        this.scoreViewTest.update();
        assertEquals(testCurrentScore.toString(),
                this.scoreViewTest.getCurrentScoreProperty().toString());
    }

    /**
     * Test for the getSortedBy() function. This will check that it returns the
     * correct score text
     */
    @Test
    public void getSortedBy() {
        // Check that is is sorted by name by default
        StringProperty testSortTitle = new SimpleStringProperty();
        testSortTitle.set("Sorted by Name");
        assertEquals(testSortTitle.toString(),
                this.scoreViewTest.getSortedBy().toString());

        // Flip it and check again
        this.scoreViewTest.switchScoreOrder();
        this.scoreViewTest.update();
        testSortTitle.set("Sorted by Score");
        assertEquals(testSortTitle.toString(),
                this.scoreViewTest.getSortedBy().toString());
    }

    /**
     * Test for the getScores() function. This will check that the
     * ObservableList that is being returned is correct.
     */
    @Test
    public void getScores() {
        // Check that the default works
        this.scoreViewTest.update();
        ObservableList<String> scoreListTest = FXCollections
                .observableArrayList();
        assertEquals(scoreListTest, this.scoreViewTest.getScores());

        // Add scores then test again and check it is sorted by name
        this.pacmanGameTest.getScores().setScore("Usagi", 200);
        this.pacmanGameTest.getScores().setScore("rei", 50);
        this.pacmanGameTest.getScores().setScore("1235", 60);
        this.scoreViewTest.update();
        scoreListTest.setAll("1235 : 60", "Usagi : 200", "rei : 50");
        assertEquals(scoreListTest, this.scoreViewTest.getScores());

        // Change the sorting order to by scores and check again
        this.scoreViewTest.switchScoreOrder();
        this.scoreViewTest.update();
        scoreListTest.setAll("Usagi : 200", "1235 : 60", "rei : 50");
        assertEquals(scoreListTest, this.scoreViewTest.getScores());
    }

    /**
     * Test for the getCurrentScore() function. This will check that the
     * current score is being returned correctly.
     */
    @Test
    public void getCurrentScore() {
        // Check the default
        assertEquals(0, this.scoreViewTest.getCurrentScore());

        // Change the value and rerun the test
        this.pacmanGameTest.getScores().increaseScore(28);
        this.scoreViewTest.update();
        assertEquals(28, this.scoreViewTest.getCurrentScore());

        // Finally, reset and check the score resets
        this.pacmanGameTest.getScores().reset();
        this.scoreViewTest.update();
        assertEquals(0, this.scoreViewTest.getCurrentScore());
    }

    /**
     * Test for the setPlayerScore() function. This will check that the score
     * is being set correctly under a normal circumstance.
     */
    @Test
    public void setPlayerScoreDefault() {
        // Check that it actually sets a score
        this.scoreViewTest.setPlayerScore("Usagi", 50);
        List<String> scoreTestList = new ArrayList<String>();
        scoreTestList.add("Usagi : 50");
        assertEquals(scoreTestList,
                this.pacmanGameTest.getScores().getEntriesByName());

        // Add another two elements to the list
        this.scoreViewTest.setPlayerScore("casper", 20);
        this.scoreViewTest.setPlayerScore("score", 560);
        scoreTestList.add("casper : 20");
        scoreTestList.add("score : 560");
        assertEquals(scoreTestList,
                this.pacmanGameTest.getScores().getEntriesByName());
    }

    /**
     * Test for the setPlayerScore() function. This will check that the score
     * is being set correctly under some of the advanced scenarios such as
     * overriding previous scores  and ignoring invalid names
     */
    @Test
    public void setPlayerScoreAdvanced() {
        // Test that it overrides the previous score with the same name (lower
        // score test)
        this.scoreViewTest.setPlayerScore("Usagi", 50);
        this.scoreViewTest.setPlayerScore("Usagi", 20);
        List<String> scoreTestList = new ArrayList<String>();
        scoreTestList.add("Usagi : 20");
        assertEquals(scoreTestList,
                this.pacmanGameTest.getScores().getEntriesByName());

        // The same test as above but when the score is higher
        this.scoreViewTest.setPlayerScore("Usagi", 100000);
        scoreTestList.set(0, "Usagi : 100000");
        assertEquals(scoreTestList,
                this.pacmanGameTest.getScores().getEntriesByName());

        // Test that it ignores invalid names
        this.scoreViewTest.setPlayerScore("!#%#@@#", 500);
        assertEquals(scoreTestList,
                this.pacmanGameTest.getScores().getEntriesByName());
    }

}
