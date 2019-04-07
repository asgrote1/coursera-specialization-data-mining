# Submission
# You must upload an image of your visualization for peer evaluation.
# In addition to your visualization, please include a paragraph that helps explain your submission. A few questions that your paragraph could answer include:
#     1. What is the data set that you chose? Why?
#
#     2. Did you use a subset of the data? If so, what was it?
#
#     3. Are there any particular aspects of your visualization to which you would like to bring attention?
#
#     4. What do you think the data and your visualization show?
#
# I've learned the hard way that R is not the tool to use for constructing
# many of the standard network graphs. However, it functions well to create maps.
#
#     1.  What is the data that you chose? Why?
#
#         The data can be found here: http://kateto.net/network-visualization.
#         This chart is modeled heavily off of Katya Ognyanova's final example
#         at the aforementioned link, but it goes several steps further and
#         focuses on airports with few connections rather than many. The data
#         were chosen after hitting a dead end with patent citation data and a
#         handful of other data sets.
#
#     2.  Did you use a subset of the data? If so, what was it?
#
#         Yes. I restricted the data to airports that fly to 5 or fewer
#         airports.
#
#     3.  Are there any particular aspects of your visualization to which you
#         would like to bring attention?
#
#         Contrast makes it easy to discern the different chart elements.
#         Moreover, I took advantage of size and color to differentiate
#         between airports with many (5) or few (1) connections via outbound
#         flights.
#
#     4.  What do you think the data and your visualization show?
#
#         Notice that there are a surprising number of long-distance flights
#         to and from small airports. This was a surprise (assuming the
#         underlying data are correct).

install.packages("maps")
install.packages("geosphere")
install.packages("Cairo")

library("maps")
library("geosphere")
library("Cairo")

draw_on_device <- FALSE


# Star graphics device. This will print to the specified file rather than
# the RStudio plots panel.

if (!draw_on_device)
    png(filename = "figures/assignment2.png",
        type = c("cairo-png"))


# Load airport data

airports <- read.csv(
    file = "data/raw/Dataset3-Airlines-NODES.csv",
    header = TRUE,
    sep = ","
)

head(airports)


flights <- read.csv(
    file = "data/raw/Dataset3-Airlines-EDGES.csv",
    header = TRUE,
    sep = ","
)

head(flights)


# Append the number of connections to the airports directory.

tab <- table(flights$Source)

airports <- airports[airports$ID %in% unique(flights$Source), ]

airports$Connections <- tab


# Keep only airports with flights out to fewer than 5 other aiports.

idx <- names(tab)[tab <= 5]

flights <- flights[flights$Source %in% idx & flights$Target %in% idx, ]

airports <- airports[airports$ID %in% unique(c(flights$Source, flights$Target)), ]


# Create color scheme for aiport colors:
#   *   grey: is few connections
#   *   orange: is many
col_beg <- "grey70"
#col_beg <- "orange_red"

col_end <- "orange"

col_fn <- colorRampPalette(c(col_beg, col_end))

airports$Color <- col_fn(10)[as.numeric(cut(airports$Connections, breaks = 10))]


# Plot a map of the united states dark color, appealing contrast and
# overlay the airports. The size of the dot depends on the number of outbound
# connections: 1-5.

map("state", col = "grey20", fill = TRUE, bg = "black", lwd = 0.1)

points(x = airports$longitude,
       y = airports$latitude,
       pch = 19,
       cex = as.double(airports$Connections) / max(airports$Connections) * 1.25,
       col = airports$Color)


# Plot the flight paths. The color scheme is similar to that used for the
# airports, with low-frequency flights being grey and high-frequency flights
# being orange. The difference is that the flight paths are transparent.

col_beg_tran <- adjustcolor(col_beg, alpha=0.4)

col_end_tran <- adjustcolor(col_end, alpha=0.4)

edge_col_fn <- colorRampPalette(c(col_beg_tran, col_end_tran), alpha = TRUE)

edge_col <- edge_col_fn(100)

for(i in 1:nrow(flights)) {

    node1 <- airports[airports$ID == flights[i, ]$Source, ]

    node2 <- airports[airports$ID == flights[i, ]$Target, ]

    arc <- gcIntermediate(p1 = c(node1[1, ]$longitude, node1[1, ]$latitude),
                          p2 = c(node2[1, ]$longitude, node2[1, ]$latitude),
                          n = 1000,
                          addStartEnd = TRUE )

    edge_intensity <- round(100 * flights[i, ]$Freq / max(flights$Freq))

    lines(arc, col = edge_col[edge_intensity], lwd = edge_intensity / 30)

}


# Add a legend and title to the plot.

idx <- order(unique(airports$Connections))

legend("bottomleft",
       legend = unique(airports$Connections)[idx],
       title = "Destinations (#)",
       bg = "black",
       text.col = "white",
       pt.bg = unique(airports$Color)[idx],
       pt.cex = seq(1, 5) / 5.0 * 1.25,
       pch = 21,
       cex = .8,
       bty = "n",
       ncol = 5)

title(main ="Airports Flying to 5 or Fewer Destinations",
      col.main = "white")


# Save the plot and disable the graphics device.

if (!draw_on_device)
    dev.off()
