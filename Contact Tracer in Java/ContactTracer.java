import java.util.*;

public class ContactTracer {
    /**
     * Variables to be used throughout this class
     */
    private Map<String, List<Trace>> contactTrace;
    private Set<String> contactsFound;


    /**
     * Initialises an empty ContactTracer with no populated contact traces.
     */
    public ContactTracer() {
        // Initialise the map to be used for contactTracer
        contactTrace = new HashMap<>();
    }

    /**
     * Initialises the ContactTracer and populates the internal data structures
     * with the given list of contract traces.
     * 
     * @param traces to populate with
     * @require traces != null
     */
    public ContactTracer(List<Trace> traces) {
        // Initialise the map to be used for contactTracer
        contactTrace = new HashMap<>();

        // Now, let's populate the map using a for loop
        for (int i = 0; i < traces.size(); i++) {
            // Check if the first person is already in the map
            if (contactTrace.containsKey(traces.get(i).getPerson1()) == false) {
                // Add this person to the map
                contactTrace.put(traces.get(i).getPerson1(), new LinkedList<Trace>());
            }

            // Check if the second person is already in the map
            if (contactTrace.containsKey(traces.get(i).getPerson2()) == false) {
                // Add this person to the map
                contactTrace.put(traces.get(i).getPerson2(), new LinkedList<Trace>());
            }

            // Now, add the trace information to both of them
            contactTrace.get(traces.get(i).getPerson1()).add(traces.get(i));
            contactTrace.get(traces.get(i).getPerson2()).add(traces.get(i));
        }
    }

    /**
     * Adds a new contact trace to 
     * 
     * If a contact trace involving the same two people at the exact same time is
     * already stored, do nothing.
     * 
     * @param trace to add
     * @require trace != null
     */
    public void addTrace(Trace trace) {
        // Add the new trace information to the map
        // First check if the first person is already in the map
        if (contactTrace.containsKey(trace.getPerson1()) == false) {
            // Add this person to the map
            contactTrace.put(trace.getPerson1(), new LinkedList<Trace>());
        }

        // Check if the second person is already in the map
        if (contactTrace.containsKey(trace.getPerson2()) == false) {
            // Add this person to the map
            contactTrace.put(trace.getPerson2(), new LinkedList<Trace>());
        }

        // Now, add the trace information to both of them
        contactTrace.get(trace.getPerson1()).add(trace);
        contactTrace.get(trace.getPerson2()).add(trace);
    }

    /**
     * Gets a list of times that person1 and person2 have come into direct 
     * contact (as per the tracing data).
     *
     * If the two people haven't come into contact before, an empty list is returned.
     * 
     * Otherwise the list should be sorted in ascending order.
     * 
     * @param person1
     * @param person2
     * @return a list of contact times, in ascending order.
     * @require person1 != null && person2 != null
     */
    public List<Integer> getContactTimes(String person1, String person2) {
        // Check that person1 and person2 are both contained in the map
        if (contactTrace.containsKey(person1) == false ||
                contactTrace.containsKey(person2) == false) {
            return List.of();
        }

        // To do this, let's get the list of traces from person1 and make a variable
        // to store the output
        List<Trace> contactPerson1 = contactTrace.get(person1);
        List<Integer> contactTimes = new LinkedList<Integer>();

        // Now, let's go through the list and get all the times person2 is in it
        for (int i = 0; i < contactPerson1.size(); i++) {
            // Check if person2 matches the person in the list
             if (contactPerson1.get(i).getPerson2().equals(person2) ||
                contactPerson1.get(i).getPerson1().equals(person2)) {
                 // If they are equal, add the contact time
                 contactTimes.add(contactPerson1.get(i).getTime());
             }
        }

        // Now that the list is full of times, let's sort it in ascending order
        contactTimes.sort(null);

        // Return the list of times
        return contactTimes;
    }

    /**
     * Gets all the people that the given person has been in direct contact with
     * over the entire history of the tracing dataset.
     * 
     * @param person to list direct contacts of
     * @return set of the person's direct contacts
     */
    public Set<String> getContacts(String person) {
        // Check that person is contained in the map
        if (contactTrace.containsKey(person) == false) {
            return Set.of();
        }

        // To do this, let's get the list of traces from person and make a variable
        // to store the output
        List<Trace> personTraces = contactTrace.get(person);
        Set<String> contacts = new HashSet<String>();

        // Now, let's go through the list and get all the contacts in it
        for (int i = 0; i < personTraces.size(); i++) {
            // First, let's find if the contact is person1 or person2 in the trace
            String person1 = personTraces.get(i).getPerson1();
            String person2 = personTraces.get(i).getPerson2();
            if (person1.equals(person)) {
                // This means that person2 is the person needed to add to the set.
                // Now, check if this person is already in set
                if (!contacts.contains(person2)) {
                    // If person2 is not in the set, add them
                    contacts.add(person2);
                }

            } else {
                // It is person1 that needs to be checked. Check if person1 is already
                // in set
                if (!contacts.contains(person1)) {
                    // If person1 is not in the set, add them
                    contacts.add(person1);
                }
            }
        }

        // Return the list of people
        return contacts;
    }

    /**
     * Gets all the people that the given person has been in direct contact with
     * at OR after the given timestamp (i.e. inclusive).
     * 
     * @param person to list direct contacts of
     * @param timestamp to filter contacts being at or after
     * @return set of the person's direct contacts at or after the timestamp
     */
    public Set<String> getContactsAfter(String person, int timestamp) {
        // Check that person is contained in the map
        if (contactTrace.containsKey(person) == false) {
            return Set.of();
        }

        // To do this, let's get the list of traces from person and make a variable
        // to store the output
        List<Trace> personTraces = contactTrace.get(person);
        Set<String> contacts = new HashSet<String>();

        // Now, let's go through the list and get all the contacts in it
        for (int i = 0; i < personTraces.size(); i++) {
            // First, let's find if the contact is person1 or person2 in the trace
            String person1 = personTraces.get(i).getPerson1();
            String person2 = personTraces.get(i).getPerson2();
            int checkTimestamp = personTraces.get(i).getTime();
            if (person1.equals(person)) {
                // This means that person2 is the person needed to add to the set.
                // Now, let's check the timestamp and compare
                if (checkTimestamp >= timestamp) {
                    // Now, check if this person is already in set
                    if (!contacts.contains(person2)) {
                        // Add the person to the set
                        contacts.add(person2);
                    }
                }

            } else {
                // It is person1 that needs to be checked. Let's check the timestamp
                // and compare
                if (checkTimestamp >= timestamp) {
                    // Now, check if this person is already in set
                    if (!contacts.contains(person1)) {
                        // Add the person to the set
                        contacts.add(person1);
                    }
                }
            }
        }

        // Return the list of people
        return contacts;
    }

    /**
     * Initiates a contact trace starting with the given person, who
     * became contagious at timeOfContagion.
     * 
     * Note that the return set shouldn't include the original person the trace started from.
     * 
     * @param person to start contact tracing from
     * @param timeOfContagion the exact time person became contagious
     * @return set of people who may have contracted the disease, originating from person
     */
    public Set<String> contactTrace(String person, int timeOfContagion) {
        // Check that person is contained in the map
        if (contactTrace.containsKey(person) == false) {
            return Set.of();
        }

        // Reset the contactsFound which are the ones to be returned
        contactsFound = new HashSet<String>();

        // Call the helper function which will do the recursion.
        Set<String> allContactTraced = contactTraceHelper(person, timeOfContagion);

        // Remove the original person as this needs to be excluded
        allContactTraced.remove(person);

        // Return the contactList
        return allContactTraced;
    }

    /**
     * This is a helper function for contactTrace() and its purpose is to do the recursion
     * for that function. It takes an input person and time and finds all the contacts
     * which occur after that time and returns it in a set.
     *
     * @param person to start contact tracing from
     * @param timeOfContagion the exact time person became contagious
     * @return set of people who may have contracted the disease, originating from person
     */
    public Set<String> contactTraceHelper(String person, int timeOfContagion) {
        // First, let's get all people who came into contact with the person after the time
        Set<String> contactsAfterTime = getContactsAfter(person, timeOfContagion);

        // Add the current person to contactsFound
        contactsFound.add(person);

        // First, let's check if the Set is empty (meaning no times are left)
        if (contactsAfterTime.isEmpty()) {
            // Return empty set
            return Set.of();
        }

        // Now, let's call the recursion if it is not empty
        Object[] contacts = contactsAfterTime.toArray();
        //Set<String> contactsFound = new HashSet<String>();
        for (int i = 0; i < contactsAfterTime.size(); i++) {
            // Now, let's get the time of contact
            List<Integer> contactTimes = getContactTimes(person, (String) contacts[i]);

            // Go through and find the first time after they meet
            int firstContact = 0;
            int foundContact = 0;
            for (int j = 0; j < contactTimes.size(); j++) {
                // Check if the contact is after timestamp and their first contact
                // hasn't already been found
                if (contactTimes.get(j) >= timeOfContagion && foundContact == 0) {
                    // Set this to be the time and mark the contact as found
                    foundContact = 1;
                    firstContact = contactTimes.get(j);
                }
            }

            // Check if the person is already in the list
            if (!contactsFound.contains(contacts[i])) {
                // Get the set containing all the people who come into contact with
                // this person after their time + 60
                Set<String> tempContacts = contactTraceHelper((String) contacts[i],
                        firstContact + 60);

                // Add all the tempContacts to the contactsFound if not null
                if (!tempContacts.isEmpty()) {
                    // Add to contactsFound
                    contactsFound.addAll(tempContacts);
                }
            }
        }

        // Return the contacts
        return contactsFound;
    }
}
