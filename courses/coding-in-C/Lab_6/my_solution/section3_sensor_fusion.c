/*
 * Lab 6 - Section III: Sensor Fusion
 *
 * Processes time-discrete sensor measurements and detects object occurrences
 * based on sensor-specific thresholds. Identifies overlapping detections
 * from multiple sensors (sensor fusion).
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SAMPLES 3000
#define FILENAME_LENGTH 100

/*
 * Structure representing a single sensor measurement
 */
typedef struct {
    float time;              /* Time of measurement in seconds */
    double probability;      /* Measured signal (0.0 to 1.0) */
} SensorData;

/*
 * Structure representing a complete sensor with all measurements
 */
typedef struct {
    int id;                              /* Sensor identifier */
    double threshold;                    /* Detection threshold */
    SensorData data[MAX_SAMPLES];        /* Array of measurements */
    int object_detection[MAX_SAMPLES];   /* Binary detection signal (0 or 1) */
    int sample_count;                    /* Number of valid samples */
} Sensor;

/*
 * Structure representing a detection interval
 */
typedef struct {
    float start_time;    /* Start time of detection in seconds */
    float end_time;      /* End time of detection in seconds */
} DetectionInterval;

/*
 * Read sensor data from a text file
 *
 * Parameters:
 *   filename: Path to the input file
 *   sensor: Pointer to Sensor struct to fill
 *
 * Returns: 0 on success, -1 on error
 */
int read_sensor_data(const char *filename, Sensor *sensor) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error: Could not open file '%s'\n", filename);
        return -1;
    }

    sensor->sample_count = 0;

    float time;
    double probability;

    while (fscanf(file, "%f %lf", &time, &probability) == 2) {
        if (sensor->sample_count >= MAX_SAMPLES) {
            printf("Warning: Maximum sample count reached (%d)\n", MAX_SAMPLES);
            break;
        }

        sensor->data[sensor->sample_count].time = time;
        sensor->data[sensor->sample_count].probability = probability;
        sensor->sample_count++;
    }

    fclose(file);

    printf("Read %d samples from '%s'\n", sensor->sample_count, filename);
    return 0;
}

/*
 * Generate binary detection signal based on threshold
 *
 * For each sample, sets object_detection[i] to:
 *   1 if probability > threshold
 *   0 otherwise
 *
 * Parameters:
 *   sensor: Pointer to Sensor struct
 */
void generate_detection_signal(Sensor *sensor) {
    for (int i = 0; i < sensor->sample_count; i++) {
        if (sensor->data[i].probability > sensor->threshold) {
            sensor->object_detection[i] = 1;
        } else {
            sensor->object_detection[i] = 0;
        }
    }
}

/*
 * Extract detection intervals from binary detection signal
 *
 * Identifies continuous intervals where object_detection == 1
 *
 * Parameters:
 *   sensor: Pointer to Sensor struct
 *   intervals: Array to store found intervals
 *   max_intervals: Maximum number of intervals to store
 *
 * Returns: Number of intervals found
 */
int extract_detection_intervals(const Sensor *sensor,
                                 DetectionInterval *intervals,
                                 int max_intervals) {
    int interval_count = 0;
    int in_detection = 0;  /* State: are we currently in a detection? */
    float start_time = 0.0f;

    for (int i = 0; i < sensor->sample_count; i++) {
        if (sensor->object_detection[i] == 1 && !in_detection) {
            /* Detection starts: 0 -> 1 transition */
            start_time = sensor->data[i].time;
            in_detection = 1;
        } else if (sensor->object_detection[i] == 0 && in_detection) {
            /* Detection ends: 1 -> 0 transition */
            /* End time is the previous sample (last time we had detection) */
            float end_time = sensor->data[i - 1].time;

            if (interval_count < max_intervals) {
                intervals[interval_count].start_time = start_time;
                intervals[interval_count].end_time = end_time;
                interval_count++;
            }

            in_detection = 0;
        }
    }

    /* Handle case where detection extends to the end of data */
    if (in_detection && interval_count < max_intervals) {
        intervals[interval_count].start_time = start_time;
        intervals[interval_count].end_time = sensor->data[sensor->sample_count - 1].time;
        interval_count++;
    }

    return interval_count;
}

/*
 * Compute overlapping detection intervals (sensor fusion)
 *
 * Finds time intervals where both sensors detect an object simultaneously
 *
 * Parameters:
 *   sensor1, sensor2: Pointers to the two sensors
 *   fused_intervals: Array to store overlapping intervals
 *   max_intervals: Maximum number of intervals to store
 *
 * Returns: Number of fused intervals found
 */
int compute_fused_intervals(const Sensor *sensor1,
                             const Sensor *sensor2,
                             DetectionInterval *fused_intervals,
                             int max_intervals) {
    int fused_count = 0;
    int in_fusion = 0;
    float start_time = 0.0f;

    /* Both sensors should have the same sample count and time stamps */
    int min_samples = (sensor1->sample_count < sensor2->sample_count) ?
                      sensor1->sample_count : sensor2->sample_count;

    for (int i = 0; i < min_samples; i++) {
        int both_detect = sensor1->object_detection[i] && sensor2->object_detection[i];

        if (both_detect && !in_fusion) {
            /* Fusion starts */
            start_time = sensor1->data[i].time;
            in_fusion = 1;
        } else if (!both_detect && in_fusion) {
            /* Fusion ends */
            float end_time = sensor1->data[i - 1].time;

            if (fused_count < max_intervals) {
                fused_intervals[fused_count].start_time = start_time;
                fused_intervals[fused_count].end_time = end_time;
                fused_count++;
            }

            in_fusion = 0;
        }
    }

    /* Handle case where fusion extends to end of data */
    if (in_fusion && fused_count < max_intervals) {
        fused_intervals[fused_count].start_time = start_time;
        fused_intervals[fused_count].end_time = sensor1->data[min_samples - 1].time;
        fused_count++;
    }

    return fused_count;
}

/*
 * Print detection intervals in formatted output
 *
 * Parameters:
 *   title: Description of the intervals (e.g., "Sensor 1 detections")
 *   intervals: Array of detection intervals
 *   count: Number of intervals
 */
void print_intervals(const char *title,
                     const DetectionInterval *intervals,
                     int count) {
    printf("%s:\n", title);

    if (count == 0) {
        printf("  No detections\n");
    } else {
        for (int i = 0; i < count; i++) {
            printf("  Start: %.2f s  End: %.2f s\n",
                   intervals[i].start_time, intervals[i].end_time);
        }
    }
    printf("\n");
}

/*
 * Main sensor fusion program
 */
int main(void) {
    printf("Lab 6 - Section III: Sensor Fusion\n");
    printf("===================================\n\n");
    Sensor sensor1, sensor2;

    sensor1.id = 1;
    sensor1.threshold = 0.8;
    sensor1.sample_count = 0;

    sensor2.id = 2;
    sensor2.threshold = 0.7;
    sensor2.sample_count = 0;
    printf("Loading sensor data...\n");
    if (read_sensor_data("sensor1.txt", &sensor1) != 0) {
        printf("Failed to read sensor 1 data\n");
        return 1;
    }

    if (read_sensor_data("sensor2.txt", &sensor2) != 0) {
        printf("Failed to read sensor 2 data\n");
        return 1;
    }

    printf("\n");
    printf("Generating detection signals...\n");
    generate_detection_signal(&sensor1);
    generate_detection_signal(&sensor2);
    printf("Detection signals generated\n\n");
    DetectionInterval intervals_sensor1[100];
    DetectionInterval intervals_sensor2[100];
    DetectionInterval fused_intervals[100];

    int count1 = extract_detection_intervals(&sensor1, intervals_sensor1, 100);
    int count2 = extract_detection_intervals(&sensor2, intervals_sensor2, 100);
    int count_fused = compute_fused_intervals(&sensor1, &sensor2, fused_intervals, 100);
    printf("--- Object Detection Results ---\n\n");

    print_intervals("Sensor 1 detections", intervals_sensor1, count1);
    print_intervals("Sensor 2 detections", intervals_sensor2, count2);
    print_intervals("Fused signal (both sensors)", fused_intervals, count_fused);

    printf("Analysis complete.\n");
    printf("Total detections: Sensor 1 = %d, Sensor 2 = %d, Fused = %d\n",
           count1, count2, count_fused);

    return 0;
}
