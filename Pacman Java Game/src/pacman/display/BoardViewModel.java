package pacman.display;

import javafx.util.Pair;
import pacman.board.PacmanBoard;
import pacman.game.PacmanGame;
import pacman.ghost.Ghost;
import pacman.ghost.Phase;
import pacman.hunter.Speedy;
import pacman.util.Position;

import java.util.ArrayList;
import java.util.List;

/**
 * BoardViewModel
 *
 * Is the intermediary between BoardView and the PacmanGame
 */
public class BoardViewModel {

    // Initiate the PacmanGame which will be used
    private PacmanGame model;

    /**
     * Constructs a new BoardViewModel to model the given PacmanGame
     *
     * @param model the game to be played
     * @require model != null
     */
    public BoardViewModel(PacmanGame model) {
        this.model = model;
    }

    /**
     * Returns the number of lives left for the player in the game.
     *
     * @return the number of lives
     */
    public int getLives() {
        return this.model.getLives();
    }

    /**
     * Returns the current level that the player is on
     *
     * @return the current level of the game
     */
    public int getLevel() {
        return this.model.getLevel();
    }

    /**
     * Returns a colour string to represent how the hunter should be displayed.
     * If game's hunter special is active it should return '#CDC3FF', otherwise
     * return '#FFE709"
     *
     * @return the colour associated with the game's hunter
     */
    public String getPacmanColour() {
        // Check if the special is active
        if (this.model.getHunter().isSpecialActive()) {
            // If special is active, return '#CDC3FF'
            return "#CDC3FF";
        }

        // Else, return #FFE709
        return "#FFE709";
    }

    /**
     * Returns the starting angle of the mouth arc of the pacman depending on
     * its direction. If the hunter's direction is RIGHT, return 30.
     * If the hunter's direction if UP, return 120.
     * If the hunter's direction is LEFT, return 210.
     * If the hunter's direction is DOWN, return 300.
     *
     * @return the angle based on the direction of the game's hunter
     */
    public int getPacmanMouthAngle() {
        // Use a switch statement to return the angle of the mouth
        switch (this.model.getHunter().getDirection()) {
            case LEFT:
                return 210;
            case UP:
                return 120;
            case RIGHT:
                return 30;
            case DOWN:
                return 300;
            default:
                // default will not run
                return 0;
        }
    }

    /**
     * Returns the current position of the game's hunter.
     *
     * @return the position of the hunter
     */
    public Position getPacmanPosition() {
        return this.model.getHunter().getPosition();
    }

    /**
     * Returns the board associated with the game.
     *
     * @return the game board
     */
    public PacmanBoard getBoard() {
        return this.model.getBoard();
    }

    /**
     * Returns the positions and colours of the ghosts in the game. Each ghost
     * should be represented as a Pair(position, colour), where position is
     * their current position on the board, and colour is their colour. The
     * ghost's colour should be given be by Ghost.getColour(). However, if the
     * ghost's phase is FRIGHTENED, this colour should instead be "#0000FF".
     *
     * @return a list of Pair<position, colour > representing the ghosts in the
     * game, in no particular order
     */
    public List<Pair<Position,String>> getGhosts() {
        // Get the ghost colour using the helper and set it to variables
        String blinkyColour = getGhostColour(this.model.getGhosts().get(0));
        String clydeColour = getGhostColour(this.model.getGhosts().get(1));
        String inkyColour = getGhostColour(this.model.getGhosts().get(2));
        String pinkyColour = getGhostColour(this.model.getGhosts().get(3));

        // Set the position of the ghost to the variables
        Position blinkyPosition = this.model.getGhosts().get(0).getPosition();
        Position clydePosition = this.model.getGhosts().get(1).getPosition();
        Position inkyPosition = this.model.getGhosts().get(2).getPosition();
        Position pinkyPosition = this.model.getGhosts().get(3).getPosition();

        // Create the pair for the ghost that will be in the list
        Pair<Position,String> blinkyPair = new Pair<>(blinkyPosition,
                blinkyColour);
        Pair<Position,String> clydePair = new Pair<>(clydePosition,
                clydeColour);
        Pair<Position,String> inkyPair = new Pair<>(inkyPosition, inkyColour);
        Pair<Position,String> pinkyPair = new Pair<>(pinkyPosition,
                pinkyColour);


        // Create the list to be returned
        List<Pair<Position,String>> ghostList = new
                ArrayList<Pair<Position,String>>();
        ghostList.addAll(
                List.of(blinkyPair, clydePair, inkyPair, pinkyPair));

        // Return the list
        return ghostList;
    }

    /**
     * Returns the colour of the ghost.  If the ghost is in the phase
     * FRIGHTENED, the colour will be "#0000FF".
     *
     * @param ghost the ghost that will have its colour returned
     * @return the colour of the ghost
     */
    private static String getGhostColour(Ghost ghost) {
        // Check if the ghost is in FRIGHTENED and return the correct colour
        if (ghost.getPhase() == Phase.FRIGHTENED) {
            return "#0000FF";
        }

        // Return the ghost colour normally if its not in FRIGHTENED
        return ghost.getColour();
    }

}
