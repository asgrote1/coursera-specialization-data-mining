# ASSIGNMENT PROMPT:  In addition to your visualization, please include a
# paragraph that helps explain your submission. A few questions that your
# paragraph could answer include:
#
#     1.    What are your x- and y-axes?
#     2.    Did you use a subset of the data? If so, what was it?
#     3.    Are there any particular aspects of your visualization to which you
#           would like to bring attention?
#     4.    What do you think the data and your visualization show?
#
# I took advantage of the provided data and aggregated the global temperature
# deviations by decade, calculating the average temperature deviation, maximum
# deviation, and minimum deviation for each. The average was used to construct a
# bar chart, with the max and min used to overlay error bars. My x-axis
# represents decades as a nominal variable, while the y-axis is temperature
# deviation in 0.01 degrees (Celsius?).
#
# I took care to maintain a reasonably minimal design implemented via ggplot2 in
# R; low saturation fill colors with full saturation borders and error bars,
# no tick marks along the axes, no x-axis label as context renders it unnecessary,
# etc. The chart makes clear that the trend is rising temperature, even when
# accounting for the dispersion of deviations throughout each decade.

library("packrat")
packrat::init()
