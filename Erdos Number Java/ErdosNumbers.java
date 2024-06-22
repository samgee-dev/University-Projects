import java.util.*;

public class ErdosNumbers {
    /**
     * String representing Paul Erdos's name to check against. Also includes variables
     * that are used throughout the assignment
     */
    public static final String ERDOS = "Paul Erdös";
    private Map<String, List<String>> authorMap;
    private Map<String, List<String>> authorsPaperMap;
    private Map<String, List<String>> paperMap;
    private int verticesCountAuthor;
    private int verticesCountTitle;
    private Map<String, Boolean> authorCheck;
    private int erdosNumber;
    private double weightedErdosNumber;

    /**
     * Initialises the class with a list of papers and authors.
     *
     * Each element in 'papers' corresponds to a String of the form:
     * 
     * [paper name]:[author1][|author2[|...]]]
     *
     * Note that for this constructor and the below methods, authors and papers
     * are unique (i.e. there can't be multiple authors or papers with the exact same name or title).
     * 
     * @param papers List of papers and their authors
     */
    public ErdosNumbers(List<String> papers) {
        // Make the maps that will store the graphs and initialise the vertices counters
        authorMap = new HashMap<>();
        authorsPaperMap = new HashMap<>();
        authorCheck = new HashMap<>();
        paperMap = new HashMap<>();
        verticesCountAuthor = 0;
        verticesCountTitle = 0;

        // Add all the information to the map
        String title = "";
        for (int i = 0; i < papers.size(); i++) {
            // First, let's split up the string
            String[] paperAndAuthors = papers.get(i).split(":");

            // Use a for loop to go through the string
            int isAuthor = 0;
            for (String storedAuthors: paperAndAuthors) {
                // Add all the authors first as they are vertices
                 if (isAuthor == 1) {
                     // Add the authors by first breaking up the list
                     String[] authors = storedAuthors.split("\\|");

                     // Go through a loop and add authors to the map
                     for (int j = 0; j < authors.length; j++) {
                         // Check if the author doesn't already exist in the map
                         if (!authorMap.containsKey(authors[j])) {
                             // If it doesn't, add it and add to the counters and checker
                             authorMap.put(authors[j], new LinkedList<String>());
                             verticesCountAuthor++;
                             authorCheck.put(authors[j], false);
                         }

                         // Let's make a loop that goes through and adds the relations
                         for (int k = j + 1; k < authors.length; k++) {
                             // Check if we need to add a new author
                             if (!authorMap.containsKey(authors[k])) {
                                 // If the author doesn't exist, add it to the counter, checker
                                 // and graph
                                 authorMap.put(authors[k], new LinkedList<String>());
                                 verticesCountAuthor++;
                                 authorCheck.put(authors[k], false);
                             }

                             // Make the relationship
                             authorMap.get(authors[j]).add(authors[k]);
                             authorMap.get(authors[k]).add(authors[j]);
                         }

                         // Add the title and the author to the authorsPaperMap
                         if (!authorsPaperMap.containsKey(authors[j])) {
                             authorsPaperMap.put(authors[j], new LinkedList<String>());
                             verticesCountTitle++;
                         }

                         // Add the paper to the authorsPaperMap and paperMap
                         authorsPaperMap.get(authors[j]).add(title);
                         paperMap.get(title).add(authors[j]);
                     }

                 } else {
                     // store the title in a variable and add to author count
                     title = storedAuthors;
                     isAuthor++;

                     // Add a paper to the paperMap
                     if (!paperMap.containsKey(title)) {
                         paperMap.put(title, new LinkedList<String>());
                     }
                 }
            }
        }
    }
    
    /**
     * Gets all the unique papers the author has written (either solely or
     * as a co-author).
     * 
     * @param author to get the papers for.
     * @return the unique set of papers this author has written.
     */
    public Set<String> getPapers(String author) {
        // make the set which contains the information
        Set<String> papersReturn = new HashSet<String>();

        // Make a author list
        List<String> papers = authorsPaperMap.get(author);

        // Go through and aod all the authors to the set
        for (int i = 0; i < papers.size(); i++) {
            // All all the authors to the set
            papersReturn.add(papers.get(i));
        }

        // Return the set
        return papersReturn;
    }

    /**
     * Gets all the unique co-authors the author has written a paper with.
     *
     * @param author to get collaborators for
     * @return the unique co-authors the author has written with.
     */
    public Set<String> getCollaborators(String author) {
        // make the set which contains the information
        Set<String> collaborators = new HashSet<String>();

        // Make a author list
        List<String> authors = authorMap.get(author);

        // Go through and aod all the authors to the set
        for (int i = 0; i < authors.size(); i++) {
            // All all the authors to the set
            collaborators.add(authors.get(i));
        }

        // Return the set
        return collaborators;
    }

    /**
     * Checks if Erdos is connected to all other author's given as input to
     * the class constructor.
     * 
     * In other words, does every author in the dataset have an Erdos number?
     * 
     * @return the connectivity of Erdos to all other authors.
     */
    public boolean isErdosConnectedToAll() {
        // Make a clone of the authorChecker to use
        Map<String, Boolean> authorCheckerClone = new HashMap<>();
        authorCheckerClone.putAll(authorCheck);

        // Let's call the recursive helper
        connectedToAllHelper(ERDOS, authorCheckerClone);

        // Check if the checker contains a false
        if (authorCheckerClone.containsValue(false)) {
            return false;
        }

        // Return true the for loop finds nothing wrong
        return true;
    }

    /**
     * This is a helper to the isErdosConnectedToAll() function. It uses recursion to check
     * all the co-authors of the entered author and if they are all connected to Erdos. It
     * uses authorChecker to keep track of those that are connected.
     *
     * @param author the author which will have its neighbours checked
     * @param authorChecker a map that is used to keep track of the authors connected to Erdos.
     */
    private void connectedToAllHelper(String author, Map<String, Boolean> authorChecker) {
        // Since we are starting at ERDOS, we can set everything found to be true
        authorChecker.replace(author, true);

        // Get the stored String List
        List<String> adjacentAuthors = authorMap.get(author);

        // Check all the neighbours
        for (int i = 0; i < adjacentAuthors.size(); i++) {
            // Check if we have found a author which hasn't been checked yet
            if (authorChecker.get(adjacentAuthors.get(i)) == false) {
                // Recall the recursion using the author
                connectedToAllHelper(adjacentAuthors.get(i), authorChecker);
            }
        }
    }

    /**
     * Calculate the Erdos number of an author. 
     * 
     * This is defined as the length of the shortest path on a graph of paper 
     * collaborations (as explained in the assignment specification).
     * 
     * If the author isn't connected to Erdos (and in other words, doesn't have
     * a defined Erdos number), returns Integer.MAX_VALUE.
     * 
     * Note: Erdos himself has an Erdos number of 0.
     * 
     * @param author to calculate the Erdos number of
     * @return authors' Erdos number or otherwise Integer.MAX_VALUE
     */
    public int calculateErdosNumber(String author) {
        // Check if Erdos has already been found
        if (author == ERDOS) {
            // If so, return 0
            return 0;
        }
        // Make the current path map to store the current path
        Map<String, Boolean> currentPath = new HashMap<>();

        // Set erdosNumber to be MAX_VALUE and set the temp to be 1
        erdosNumber = Integer.MAX_VALUE;
        int erdosNumberTemp = 1;

        // Let's call the recursive helper
        calculateErdosNumberHelper(author, currentPath, erdosNumberTemp);

        // Return the Erdos Number
        return erdosNumber;
    }

    /**
     * This helper function uses recursion to go down all the neighbours of a author and
     * check if any of them are Erdos. If none are, it will check those neighbour's
     * neighbours and so on until it finds Erdos or doesn't find any connection. The
     * erdosNumberTemp will be used to update the erdosNumber as shorter paths are found.
     *
     * @param author to calculate the Erdos number of
     * @param currentPath a map that stores all the authors which have been visited
     * @param erdosNumberTemp a temp number which indicates how far we are down a path
     */
    private void calculateErdosNumberHelper(String author, Map<String, Boolean> currentPath,
                                            int erdosNumberTemp) {
        // Add the current author to the current path
        currentPath.put(author, true);

        // Get the stored String List
        List<String> adjacentAuthors = authorMap.get(author);

        // Check all the neighbours of the inputted author
        for (int i = 0; i < adjacentAuthors.size(); i++) {
            // Check if the neighbour is Erdos
            if (adjacentAuthors.get(i).equals(ERDOS)) {
                // If we have found ERDOS, check if the value is smaller than the current
                // fastest path
                if (erdosNumberTemp < erdosNumber) {
                    // Set the new smallest number and remove from current path
                    erdosNumber = erdosNumberTemp;
                    currentPath.remove(author);
                    return;
                }
            }
        }

        // If it was not found on any of the neighbours, we will need to add to temp
        erdosNumberTemp++;

        // If the erdosNumberTemp is already equal to or greater than the erdosNumber,
        // Don't check all the neighbouring authors
        if (erdosNumberTemp < erdosNumber) {
            // Now, let's call recursion to check the neighbour's neighbours
            for (int i = 0; i < adjacentAuthors.size(); i++) {
                // Check if we have found a author which hasn't been checked yet
                if (!(currentPath.containsKey(adjacentAuthors.get(i)))) {
                    // Recall the recursion using the author
                    calculateErdosNumberHelper(adjacentAuthors.get(i), currentPath,
                            erdosNumberTemp);
                }
            }
        }

        // Clear the path
        currentPath.remove(author);
    }

    /**
     * Gets the average Erdos number of all the authors on a paper.
     * If a paper has just a single author, this is just the author's Erdos number.
     *
     * Note: Erdos himself has an Erdos number of 0.
     *
     * @param paper to calculate it for
     * @return average Erdos number of paper's authors
     */
    public double averageErdosNumber(String paper) {
        // First, get the list of all the authors on a paper
        List<String> authors = paperMap.get(paper);

        // Create the averageErdosNumber that will get calculated
        double totalErdosNumbers = 0;

        // Go through a loop and add all the numbers to the average
        for (int i = 0; i < authors.size(); i++) {
            // Use a temp erdos number to store the current author's number.
            // Call the calculateErdosNumber to find the current author's number.
            int currentErdosNumber = calculateErdosNumber(authors.get(i));

            // Add the number to the total
            totalErdosNumbers += currentErdosNumber;
        }

        // Now, let's divide the total by the number of authors to get the average
        double averageErdosNumber = totalErdosNumbers / (authors.size());

        // Return result
        return averageErdosNumber;
    }

    /**
     * Calculates the "weighted Erdos number" of an author.
     * 
     * If the author isn't connected to Erdos (and in other words, doesn't have
     * an Erdos number), returns Double.MAX_VALUE.
     *
     * Note: Erdos himself has a weighted Erdos number of 0.
     * 
     * @param author to calculate it for
     * @return author's weighted Erdos number
     */
    public double calculateWeightedErdosNumber(String author) {
        // Check if Erdos has already been found
        if (author == ERDOS) {
            // If so, return 0
            return 0;
        }

        // Make the current path map to store the current path
        Map<String, Boolean> currentPath = new HashMap<>();

        // Set WeightedErdosNumber to be MAX_VALUE and set the temp to be 1
        weightedErdosNumber = Double.MAX_VALUE;
        double weightedErdosTemp = 0.0;

        // Let's call the recursive helper
        weightedErdosNumberHelper(author, null, currentPath, weightedErdosTemp);

        // Return the Erdos Number
        return weightedErdosNumber;
    }

    /**
     * This helper function uses recursion to go down all the neighbours of a author and
     * check if any of them are Erdos. If none are, it will check those neighbour's
     * neighbours and so on until it finds Erdos or doesn't find any connection. The
     * weightedErdosTemp will be used to update the WeightedErdosNumber as shorter paths
     * are found.
     *
     * @param author to calculate the Erdos number of
     * @param previousAuthor the previous author in the path
     * @param currentPath a map that stores all the authors which have been visited
     * @param weightedErdosTemp a temp number which indicates how far we are down the path
     *                          in terms of weighted edges
     */
    private void weightedErdosNumberHelper(String author, String previousAuthor,
                                           Map<String, Boolean> currentPath,
                                           double weightedErdosTemp) {
        // Add the current author to the current path
        currentPath.put(author, true);

        // Calculate the weighted Erdos number. First check if the previous author
        // exist
        if (previousAuthor != null) {
            // Since the previous author exists, let's calculate how many shared
            // papers the authors have. First, get the previous author's list
            List<String> previousAuthorsCollab = authorMap.get(previousAuthor);

            // Now go through list and count the number of times the new author
            // appears
            double numberOfCollab = 0;
            for (int i = 0; i < previousAuthorsCollab.size(); i++) {
                // Check if the current author is found
                if (previousAuthorsCollab.get(i).equals(author)) {
                    // Add to the number of collaborations
                    numberOfCollab++;
                }
            }

            // Now add the number of collaborations to the weighted Erdos Temp
            weightedErdosTemp += (1 / numberOfCollab);
        }

        // Now, check if Erdos was found
        if (author.equals(ERDOS)) {
            // If we have found ERDOS, check if the value is smaller than the current
            // fastest path
            if (weightedErdosTemp < weightedErdosNumber) {
                // Set the weightedErdosTemp to b
                // Set the new smallest number and remove from current path
                weightedErdosNumber = weightedErdosTemp;
                currentPath.remove(author);
                return;
            }
        }

        // Get the stored String List
        List<String> adjacentAuthors = authorMap.get(author);

        // If the weightedErdosTemp is already equal to or greater than the
        // weightedErdosNumber, don't check all the neighbouring authors
        if (weightedErdosTemp < weightedErdosNumber) {
            // Now, let's call recursion to check the neighbour's neighbours
            for (int i = 0; i < adjacentAuthors.size(); i++) {
                // Check if we have found a author which hasn't been checked yet
                if (!(currentPath.containsKey(adjacentAuthors.get(i)))) {
                    // Recall the recursion using a new author
                    weightedErdosNumberHelper(adjacentAuthors.get(i), author,
                            currentPath, weightedErdosTemp);
                }
            }
        }

        // Clear the path
        currentPath.remove(author);
    }
}
