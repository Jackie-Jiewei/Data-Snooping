# Data Snooping, Technical Trading Rules, and Bootstrap
The Data Snooping Bias is a statistical bias that appears when exhaustively searching for combinations of variables, the probability that a result arose by chance grow with the number of combinations tested. \
For example, David Leinweber, managing director of First Quadrant, discovered that historically, the single best prediction of the Standard & Poor’s 500 stock index was butter production in Bangladesh.\
Obviously, S&P 500 index has nothing to do with butter production in Bangladesh although their trend are strongly similar, which implies the danger of data snooping, especially in many areas of finance and economics.

## Universe Rules
### Filter Rules
The Filter Rules generate signals according to the percentage change in price (x %) from a subsequent high or low.  A subsequent high is the highest closing price achieved while holding a particular long position. Price moves more than x % in either direction are considered signals while moves below x % are ignored and positions maintained.
### Moving Averages
The standard Moving Averages rule utilizes the price line and the moving average of price to generates signals. Once we observed a crossover of different period of MA lines, signal is created. Moreover, in the project we add more filters into this trading rules, which includes:\
The fixed percentage band filter requires the buy or sell signal to exceed the moving average by a fixed multiplicative amount, b.\
Holding days c.\
The time delay filter requires the buy or sell signal to remain valid for a prespecified number of days, d, before action is taken.
### Support and Resistance
The Support and Resistance rule utilizes the maximum and minimum price over the previous n days to generate signals. Buy signals are shown when the price exceeds the maximum price while sell signals are shown with price below the minimum. Variations include:\
A low (high) can also be defined as the most recent closing price that is less (greater) than the e previous closing prices.\
The fixed percentage band filter b.\
Holding days c.\
The time delay filter days d.
### Channel Breakout
A channel (sometimes referred to as a trading range) can be said to occur when the high over the previous n days is within x percent of the low over the previous n days, not including the current price. \
The rules we develop for testing the channel break- out are to buy when the closing price exceeds the channel, and to sell when the price moves below the channel. \
Long and short positions are held for a fixed number of days, c. \
Additionally, a fixed percentage band, b, can be applied to the channel as a filter. 
### On-Balance Volume Averages
On-Balance Volume Averages is a volume-based indicator trading rule.\
The on-balance volume (OBV) indicator is calculated by keeping a running total of the indicator each day and adding the entire amount of daily volume when the closing price increases, and subtracting the daily volume when the closing price decreases. \
We then apply a moving average of n days to the OBV indicator. The OBV trading rules employed are the same as for the moving average trading rules, except in this case the value of interest is the OBV indicator rather than price.
## Bootstrap using White Reality Check
The biggest difference between white reality check and normal bootstrap method is that the best rules selection after resampling of the return need to be recalculated, which can largely solve the problem of data mining bias.\
The bootstrap algorithm is like follows:
1: Set t = R. Draw theta(t) = theta(R) at random, independently and uniformly from {R,...,T}.\
2: Increment t by 1. If t > T, stop. Otherwise, draw a standard normal random variable U.\
(a) If U < q, draw another random theta.\
(b) If U >= q, set theta(t) = theta(t - 1) + 1; If theta > T ,reset theta = R.
## Conclusion
1: I got the best rule MA(2) from a universe of 6000+ rules from 1950-2000, this rule lost its performance during 2000-2020.\
2: The return of best rule diminish as time goes by. The market is becoming more and more efficient.\
3: The smoothing parameter q doesn't affect stationary bootstrap a lot.\
4: The best rule is significant after accounting for the data snooping bias proved by the white reality check through stationary bootstrap.


