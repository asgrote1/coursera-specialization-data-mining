install.packages("packrat")
install.packages("scales")
install.packages("ggplot2")
install.packages("dplyr")

library("ggplot2")
library("dplyr")

data <- read.csv(
    file = "data/raw/refactored-uiuc/ExcelFormattedGISTEMPData2CSV.csv",
    header = TRUE,
    sep = ","
)

# Number of years in the data
year_num <- nrow(data)

# Earliest year found in the data
year_min <- min(data$Year)

# Most recent year found in the data
year_max <- max(data$Year)

# Test for continuity
if (year_num == year_max - year_min + 1) {
    msg_post <- "All years are present."
} else {
    msg_post <- "Uh-oh! At least one year is missing!"
}

# Report
cat("There are ",
    year_num,
    " years of data ranging from ",
    year_min,
    " to ",
    year_max,
    ". ",
    msg_post,
    sep = "")


# Add decade column as a character vector: Each decade represents one bin in
# the target bar chart.
data$Decade <- as.character(data$Year %/% 10 * 10)


# Summarize the data for each decade, providing the average deviation for the
# bar heights and the min/max deviation for the error bars.
gg_data <- data %>%
    group_by(Decade) %>%
    summarize(mn = min(Glob),
              mx = max(Glob),
              avg = mean(Glob))


# Convert the summarized data to a data.frame for compatibility with ggplot2.
gg_data <- do.call(data.frame, gg_data)


# Begin constructing the plot.
plt <- ggplot(data = gg_data,
              mapping = aes(x = Decade,
                            y = avg,
                            ymin = mn,
                            ymax = mx))

plt <- plt +

    geom_bar(stat = "identity",
             color = "#3182bd",
             fill = "#deebf7") +

    geom_errorbar(color = "#3182bd",
                  width = 0.5)

plt <- plt +

    theme_light() +

    theme(plot.title = element_text(hjust = 0.5),
          plot.caption = element_text(margin = margin(t = 10)),
          panel.grid.major.x = element_blank(),
          panel.grid.minor.x = element_blank(),
          panel.grid.minor.y = element_blank(),
          axis.ticks = element_blank(),
          axis.title.x = element_blank(),
          axis.text.x = element_text(angle = 30,
                                     vjust = .8,
                                     hjust = 0.8))

plt <- plt +
    labs(title = "Deviation of the Global Average Temperature From\nthe 1951-1980 Average, By Decade",
         caption = "Error bars show the max/min temp. deviations w/in the decade.") +
    ylab("Avg Deviation In 0.01 Degrees")

plot(plt)

ggsave("./figures/assignment1.png")
