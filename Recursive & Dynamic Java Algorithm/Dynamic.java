package assignment2;

import java.util.*;

public class Dynamic {

    /**
     * @require The number of days that you are managing the venue for, k, is greater than
     *          or equal to 1.
     * 
     *          The arrays of configurations and booking requests cannot be null and
     *          cannot contain null values.
     * 
     *          The number of configurations, configurations.size(), is greater than or
     *          equal to one. For convenience you may assume that each of the
     *          configurations has a unique identifier that corresponds to its index in
     *          the input array of configurations.
     * 
     *          The number of booking requests, bookingRequests.size(), is greater than or
     *          equal to zero. For each booking request b in bookingRequests, b.end() <=
     *          k-1. For convenience you may assume that each of the booking requests has
     *          a unique identifier that corresponds to its index in the input array of
     *          booking requests. The booking requests are not guaranteed to be sorted in
     *          any particular order.
     * 
     * @ensure Returns the maximum profit of any schedule for the venue (given the input
     *         parameters k, configurations and bookingRequests).
     * 
     *         (See handout for details.)
     * 
     *         This method must be implemented using an efficient bottom-up dynamic
     *         programming solution to the problem (not memoised).
     */
    public static int optimalProfitDynamic(int k, ArrayList<Configuration> configurations,
            ArrayList<BookingRequest> bookingRequests) {
        // Set up main variables to be used in DP solution
        int maxProfit = Integer.MIN_VALUE;
        Configuration startConfig = configurations.get(0);
        int [][] accumProfitConfig = new int [k][configurations.size()];
        BookingRequest [] bookingConfig = new BookingRequest [configurations.size()];
        int currentBookingTracker = 0;
        int storedBookingTracker = 0;
        BookingRequest currentActiveBooking = null;

        // The first loop will iterate through the days
        for (int currentDay = 0; currentDay < k; currentDay++) {
            // The next loop will iterate through the configurations
            for (Configuration currentConfig : configurations) {
                // Need to account for starting day since table needs to be filled in
                boolean validBooking = true;
                currentBookingTracker = storedBookingTracker;
                if (currentDay != 0) {
                    // This will be the main portion of the loop as the other one was
                    // just for the initialisation. First, do idle calculations on branch
                    if (accumProfitConfig[currentDay - 1][currentConfig.id()] !=
                            Integer.MIN_VALUE) {
                        accumProfitConfig[currentDay][currentConfig.id()] =
                                accumProfitConfig[currentDay - 1][currentConfig.id()] -
                                        currentConfig.cost(currentDay);
                    } else {
                        accumProfitConfig[currentDay][currentConfig.id()] =
                                Integer.MIN_VALUE;
                    }

                    // Check if new possible config path
                    for (Configuration compareConfig : configurations) {
                        // Check if there exists good reconfiguration
                        int changeTime = currentConfig.setupTime() +
                                compareConfig.teardownTime();

                        // Check configuring is possible
                        if (changeTime - 1 <= currentDay &&
                                (currentConfig != compareConfig) &&
                                bookingConfig[currentConfig.id()] == null) {
                            // Calculate if the reconfiguration cost is better
                            int setupStart = currentDay - currentConfig.setupTime() + 1;
                            int teardownStart = setupStart - compareConfig.teardownTime();
                            int setupCost = -1 * (currentConfig.cost(setupStart,
                                    currentDay));
                            int teardownCost =
                                    accumProfitConfig[teardownStart][compareConfig.id()];

                            // Check if this is a better branch and if so, set it
                            if ((setupCost + teardownCost) >
                                    accumProfitConfig[currentDay][currentConfig.id()] &&
                                    teardownCost != Integer.MIN_VALUE) {
                                accumProfitConfig[currentDay][currentConfig.id()] =
                                        setupCost + teardownCost;
                            }
                        }
                    }

                    // get currentProfit
                    int currentDailyProfit = 0;
                    if (bookingConfig[currentConfig.id()] != null) {
                        currentDailyProfit = bookingConfig[currentConfig.id()].
                                payment(currentConfig) / (bookingConfig[currentConfig.id()].
                                end() - bookingConfig[currentConfig.id()].start() + 1);
                    }

                    // Check if there exist a booking (or a better booking then the
                    // current booking)
                    while (validBooking && (currentBookingTracker <
                            bookingRequests.size())) {
                        validBooking = false;

                        // Go through the bookings (if any) for the day)
                        BookingRequest currentBooking = bookingRequests
                                .get(currentBookingTracker);
                        if (currentBooking.start() == currentDay) {
                            int testProfit = currentBooking.payment(currentConfig) /
                                    (currentBooking.end() - currentBooking.start() + 1);

                            // check if a new max is acquired
                            if (testProfit > currentDailyProfit) {
                                currentDailyProfit = testProfit;
                                bookingConfig[currentConfig.id()] = currentBooking;
                            }

                            // Iterate through booking
                            currentBookingTracker++;
                            validBooking = true;
                        }
                    }

                    // Check last config for storedBookerTracker
                    if (currentConfig.id() == configurations.size() - 1) {
                        storedBookingTracker = currentBookingTracker;
                    }

                    // Check if there exist a booking which should now be invalid
                    if (bookingConfig[currentConfig.id()] != null) {
                        // check if booking over and add profit
                        if (bookingConfig[currentConfig.id()].end() == currentDay) {
                            accumProfitConfig[currentDay][currentConfig.id()] +=
                                    bookingConfig[currentConfig.id()].payment(currentConfig);
                            bookingConfig[currentConfig.id()] = null;
                        }
                    }
                } else {
                    // The starting day case where we establish the configs
                    if (currentConfig.id() == startConfig.id()) {
                        int localProfit = 0;
                        while (validBooking && (currentBookingTracker <
                                bookingRequests.size())) {
                            validBooking = false;

                            // Go through the bookings (if any) for the day)
                            BookingRequest currentBooking = bookingRequests
                                    .get(currentBookingTracker);
                            if (currentBooking.start() == currentDay) {
                                // A booking has been found so perform the calculations and
                                // add it to the table
                                int testProfit = currentBooking.payment(currentConfig) /
                                        (currentBooking.end() - currentBooking.start() + 1);
                                // Check if there is a new profit
                                if (localProfit <= testProfit) {
                                    localProfit = testProfit;
                                    bookingConfig[currentConfig.id()] = currentBooking;
                                }

                                // Go through the booking array
                                currentBookingTracker++;
                                validBooking = true;
                            }
                        }

                        // Subtract the cost from local profit and add to array
                        storedBookingTracker = currentBookingTracker;
                        if (bookingConfig[currentConfig.id()] != null) {
                            if (bookingConfig[currentConfig.id()].end() == currentDay) {
                                accumProfitConfig[currentDay][currentConfig.id()] =
                                        bookingConfig[currentConfig.id()].
                                                payment(currentConfig);
                                bookingConfig[currentConfig.id()] = null;
                            }
                        }

                        accumProfitConfig[currentDay][currentConfig.id()] -=
                                currentConfig.cost(currentDay);
                    } else {
                        // For all the other configurations, set the value to be the min
                        // value so that the rest of the calculations can happen
                        accumProfitConfig[currentDay][currentConfig.id()] =
                                Integer.MIN_VALUE;
                    }
                }
            }
        }

        // Locate best configurations
        for (Configuration finalConfig : configurations) {
            if (accumProfitConfig[k - 1][finalConfig.id()] > maxProfit) {
                maxProfit = accumProfitConfig[k - 1][finalConfig.id()];
            }
        }
        // Return the max profit found
        return maxProfit;
    }

    /**
     * @require The number of days that you are managing the venue for, k, is greater than
     *          or equal to 1.
     * 
     *          The arrays of configurations and booking requests cannot be null and
     *          cannot contain null values.
     * 
     *          The number of configurations, configurations.size(), is greater than or
     *          equal to one. For convenience you may assume that each of the
     *          configurations has a unique identifier that corresponds to its index in
     *          the input array of configurations.
     * 
     *          The number of booking requests, bookingRequests.size(), is greater than or
     *          equal to zero. For each booking request b in bookingRequests, b.end() <=
     *          k-1. For convenience you may assume that each of the booking requests has
     *          a unique identifier that corresponds to its index in the input array of
     *          booking requests. The booking requests are not guaranteed to be sorted in
     *          any particular order.
     * 
     * @ensure Returns a schedule for the venue with the maximum profit (given the input
     *         parameters k, configurations and bookingRequests).
     * 
     *         (See handout for details.)
     * 
     *         This method must be implemented using an efficient bottom-up dynamic
     *         programming solution to the problem (not memoised).
     */
    public static Activity[] optimalScheduleDynamic(int k, ArrayList<Configuration> configurations,
            ArrayList<BookingRequest> bookingRequests) {
        return null; // REMOVE THIS LINE AND IMPLEMENT THIS METHOD
    }

}
