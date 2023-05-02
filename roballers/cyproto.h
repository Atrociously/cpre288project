#ifndef CYPROTO_H_
#define CYPROTO_H_

#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>


typedef enum CyprotoError {
  None,
  BufferOverflow,
  Postcard,
} CyprotoError;

typedef struct DriveDone {
  float total_distance;
  bool bump_detected;
  bool cliff_detected;
} DriveDone;

typedef struct DriveCommand {
  float distance;
  uint16_t speed;
} DriveCommand;

typedef struct TurnCommand {
  float angle;
  uint16_t speed;
} TurnCommand;

typedef struct ScanCommand {
  uint8_t start;
  uint8_t end;
} ScanCommand;

typedef enum CommandRequest_Tag {
  Error,
  Drive,
  Turn,
  Scan,
} CommandRequest_Tag;

typedef struct CommandRequest {
  CommandRequest_Tag tag;
  union {
    struct {
      enum CyprotoError error;
    };
    struct {
      struct DriveCommand drive;
    };
    struct {
      struct TurnCommand turn;
    };
    struct {
      struct ScanCommand scan;
    };
  };
} CommandRequest;

typedef struct ObjectData {
  float distance;
  uint8_t angle;
  float width;
} ObjectData;

typedef struct ScanDone {
  size_t size;
  const struct ObjectData *objects;
} ScanDone;

typedef struct TurnDone {
  float total_angle;
} TurnDone;

/**
 * Get the expected buffer size for serializing and deserializing data
 * make sure the buffer has exactly cyproto_buffer_size() elements
 */
size_t cyproto_buffer_size(void);

/**
 * Serialize a drive result struct into the provided buffer
 * make sure the buffer has exactly cyproto_buffer_size() elements
 */
size_t cyproto_drive_done(struct DriveDone val, uint8_t *buf);

struct CommandRequest cyproto_parse_command(uint8_t *buf);

/**
 * Serialize a scan result struct into the provided buffer
 * make sure the buffer has exactly cyproto_buffer_size() elements
 */
size_t cyproto_scan_done(struct ScanDone val, uint8_t *buf);

/**
 * Serialize a turn result struct into the provided buffer
 * make sure the buffer has exactly cyproto_buffer_size() elements
 */
size_t cyproto_turn_done(struct TurnDone val, uint8_t *buf);

/**
 * Get the maximum number of scan objects that are allowed by the buffer size
 * make sure the buffer has exactly cyproto_buffer_size() elements
 */
size_t max_objects(void);

#endif
