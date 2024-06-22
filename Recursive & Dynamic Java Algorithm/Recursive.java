package assignment2;

import java.util.*;

public class Recursive {
    /**
     * Variables to be used throughout the recursive calls.
     *      - currentVenueState has idle (0), reconfigured (1) and hosting (2)
     */
    private static int maxProfit;
    private static int venueMaxLength;

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
     *         This method must be implemented using a recursive programming solution to
     *         the problem. It is expected that your recursive algorithm will not be
     *         polynomial-time in the worst case. (You must NOT provide a dynamic
     *         programming solution to this question.)
     */
    public static int optimalProfitRecursive(int k, ArrayList<Configuration> configurations,
            ArrayList<BookingRequest> bookingRequests) {
        // Set up the variables to be used in the recursive call. currentState can either
        // be in idle (0), reconfiguration (1) or hosting (2)
        venueMaxLength = k;
        maxProfit = Integer.MIN_VALUE;
        int currentDay = 0;
        int currentConfig = configurations.get(0).id();

        // Make the recursive call
        profitRecursiveHelper(configurations, bookingRequests, currentDay, 0,
                currentConfig);

        // Return the best profit scenario
        return maxProfit;
    }

    /**
     * The helper function performs the recursive call by exploring the branches and
     * storing the branch with the highest profit.
     *
     * @param configurations an array storing all the configurations.
     * @param bookingRequests an array storing all the bookingRequests.
     * @param currentDay the currentDay of the schedule.
     * @param currentProfit the currentProfit of the schedule.
     * @param currentConfig the currentConfiguration of the schedule.
     */
    private static void profitRecursiveHelper(ArrayList<Configuration> configurations,
            ArrayList<BookingRequest> bookingRequests, int currentDay, int currentProfit,
            int currentConfig) {
        // Base case if the currentDay has reached past the final day
        if (currentDay >= venueMaxLength) {
            // Check if there is a new max profit
            if (maxProfit < currentProfit) {
                maxProfit = currentProfit;
            }

            // Exit the branch
            return;
        }

        // The main section of this recursive will be examining all possibilities
        // First, there is the reconfiguration branch
        for (Configuration nextConfig : configurations) {
            // Ignore current config as no point changing to it
            if (nextConfig.id() != currentConfig) {
                // Calculate the time of tear down and set up
                int teardownTime = configurations.get(currentConfig).teardownTime();
                int setupTime = configurations.get(nextConfig.id()).setupTime();
                int teardownDayEnd = currentDay + teardownTime - 1;
                int setupCompletionDay = currentDay + teardownTime + setupTime - 1;

                // Now calculate the cost
                int teardownCost = configurations.get(currentConfig).cost(currentDay,
                        teardownDayEnd);
                int setupCost = nextConfig.cost(teardownDayEnd + 1, setupCompletionDay);
                int newCurrentProfit = currentProfit - (teardownCost + setupCost);

                // Update day and return recursive
                profitRecursiveHelper(configurations, bookingRequests,
                        setupCompletionDay + 1, newCurrentProfit,
                        nextConfig.id());
            }
        }

        // Next, there is going to be if the booking section
        for (BookingRequest potentialBooking : bookingRequests) {
            // Check we can perform the booking
            if (currentDay == potentialBooking.start()) {
                // Perform all the calculations for said booking and do recursive call
                int newCurrentProfit = currentProfit +
                        potentialBooking.payment(configurations.get(currentConfig));
                newCurrentProfit = newCurrentProfit - configurations.get(currentConfig).
                        cost(currentDay, potentialBooking.end());
                profitRecursiveHelper(configurations, bookingRequests,
                        potentialBooking.end() + 1, newCurrentProfit,
                        currentConfig);
            }
        }

       // Finally, there will be the case if it remains idle
        currentProfit -= configurations.get(currentConfig).cost(currentDay);
        profitRecursiveHelper(configurations, bookingRequests, currentDay + 1,
                currentProfit, currentConfig);
    }
}