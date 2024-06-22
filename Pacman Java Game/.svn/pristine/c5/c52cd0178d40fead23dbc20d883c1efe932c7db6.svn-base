package pacman.display;

import javafx.beans.property.SimpleStringProperty;
import javafx.beans.property.StringProperty;
import javafx.collections.FXCollections;
import javafx.collections.ObservableList;
import pacman.game.PacmanGame;

import java.util.List;

/**
 * ScoreViewModel
 *
 * Is an intermediary between ScoreView and the PacmanGame. Used for displaying
 * the players score in the GUI
 */
public class ScoreViewModel {

    // Initate the variables to be used
    private PacmanGame model;
    private int currentScore;
    private StringProperty currentScoreProperty;
    private Boolean isByName;
    private StringProperty sortedProperty;
    private ObservableList<String> scoreList;

    /**
     * Creates a new ScoreViewModel and updates it properties. The default
     * ordering of the scores should be by name.
     *
     * @param model the PacmanGame to link to the view
     * @require model != null
     */
    public ScoreViewModel(PacmanGame model) {
        this.model = model;
        this.currentScore = model.getScores().getScore();
        this.currentScoreProperty = new SimpleStringProperty();
        this.sortedProperty = new SimpleStringProperty();
        this.isByName = true;
        this.scoreList = FXCollections.observableArrayList();
        this.scoreList.setAll(model.getScores().getEntriesByName());
        update();
    }

    /**
     * Updates the properties containing the current score, the sort order of
     * the scoreboard and the list of sorted scores.
     *
     * The format for the current score property should be
     * "Score: [currentScore]" without quotes or brackets, where currentScore
     * is the value returned by ScoreBoard.getScore().
     *
     * The sort order property should be set to either "Sorted by Name" or
     * "Sorted by Score", depending on the current score sort order.
     *
     * Finally, the property representing the list of scores should be updated
     * to contain a list of scores sorted according to the current score sort
     * order, as returned by ScoreBoard.getEntriesByName() and
     * ScoreBoard.getEntriesByScore().
     */
    public void update() {
        // Create the current score property with the correct format
        this.currentScore = this.model.getScores().getScore();
        this.currentScoreProperty.set("Score: " + this.currentScore);

        // Call switchScoreOrder() to switch the order
        //switchScoreOrder();

        // Now, let's change the score list based on what was updated
        if (this.isByName) {
            this.scoreList.setAll(this.model.getScores().getEntriesByName());
            this.sortedProperty.set("Sorted by Name");
        } else {
            this.scoreList.setAll(this.model.getScores().getEntriesByScore());
            this.sortedProperty.set("Sorted by Score");
        }
    }

    /**
     * Changes the order in which player's scores are displayed.
     * The possible orderings are by name or by score. Calling this method
     * once should switch between these two orderings.
     */
    public void switchScoreOrder() {
        // There is a variable that will be true if it is sortedByName. Check
        // if this variable is true
        if (this.isByName) {
            // Set isByName to be false
            this.isByName = false;

        } else {
            // Set isByName to be true
            this.isByName = true;
        }
    }

    /**
     * Returns the StringProperty containing the current score for the player.
     *
     * @return the property representing the current score
     */
    public StringProperty getCurrentScoreProperty() {
        return this.currentScoreProperty;
    }

    /**
     * Returns the StringProperty of how the player's scores are displayed.
     *
     * @return StringProperty representing how the player's scores are
     * displayed
     */
    public StringProperty getSortedBy() {
        return this.sortedProperty;

    }

    /**
     * Returns a list containing all "Name : Value" score entries in the game's
     * ScoreBoard, sorted by the current sort order.
     *
     * @return the list of sorted scores
     */
    public ObservableList<String> getScores() {
        return scoreList;
    }

    /**
     * Returns the overall current score for the game.
     *
     * @return current score
     */
    public int getCurrentScore() {
        return this.currentScore;
    }

    /**
     * Sets the given player's score to score. This should override the score
     * if it was previously set (even if new score is lower). Invalid player
     * names should be ignored.
     *
     * @param player the player
     * @param score the new score
     */
    public void setPlayerScore(String player, int score) {
        // Use the setScores function in ScoreBoard to set the score
        this.model.getScores().setScore(player, score);
    }
}
