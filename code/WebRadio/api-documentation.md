# API Documentation

## Endpoints

### `GET /api/info`

**Description:**
Returns the current volume level.

**Response:**
- `200 OK`: JSON object containing the information.

**Example Response:**
```json
{
  "access_point_ssid": "",
  "temperature": "",
  "pressure": "",
  "total_free_bytes": "",
  "minimum_free_bytes": "",
  "is_connected_to_wifi": "",
  "is_connecting_to_wifi": "",
  "connected_to_ssid": "",
  "local_ip": "",
  "date_time": "",
  "date_time_configured": "",
}
```


### `GET /api/volume`

**Description:**
Returns the current volume level.

**Response:**
- `200 OK`: JSON object containing the current volume.

**Example Response:**
```json
{
  "volume": 50
}
```


### `POST /api/volume`

**Description:**
Sets the volume to a specified level.

**Request Parameters:**
- `volume` (integer, required): The new volume level (0-100).

**Response:**
- `201 Created`: Volume successfully updated.
- `400 Bad Request`: Invalid volume value.

**Example Request:**
```json
{
  "volume": 75
}
```


### `GET /api/available-networks`

**Description:**
Returns a list of available Wi-Fi networks.

**Response:**
- `200 OK`: JSON array of available networks.

**Example Response:**
```json
[
  { "ssid": "Network1", "signal": -45 },
  { "ssid": "Network2", "signal": -60 }
]
```


### `PATCH /api/streams/{index}`

**Description:**
Updates a specific stream.

**Path Parameters:**
- `index` (integer, required): The index of the stream to update.

**Request Parameters:**
- `name` (string, optional): The new name of the stream.
- `url` (string, optional): The new URL of the stream.

**Response:**
- `200 OK`: Stream successfully updated.
- `400 Bad Request`: Invalid request or index out of range.

**Example Request:**
```json
{
  "name": "New Stream Name",
  "url": "http://example.com/stream"
}
```
**Example Response:**

```json
{
  "index": 1,
  "name": "New Stream Name",
  "url": "http://example.com/stream"
}
```


### `DELETE /api/streams/{index}`

**Description:**
Deletes a specific stream.

**Path Parameters:**
- `index` (integer, required): The index of the stream to delete.

**Response:**
- `200 OK`: Stream successfully deleted.
- `400 Bad Request`: Invalid index.

**Example Response:**
```json
{
  "message": "Stream deleted successfully"
}
```


### `GET /api/streams`

**Description:**
Returns a list of all streams.

**Response:**
- `200 OK`: JSON array of streams.

**Example Response:**
```json
[
  { "index": 0, "name": "Stream1", "url": "http://example.com/1" },
  { "index": 1, "name": "Stream2", "url": "http://example.com/2" }
]
```


### `GET /api/credentials`

**Description:**
List all available credentials

**Response:**
- `200 OK`

**Example Response:**
```json
[
  { "ssid": "ssid1", "password": ""},
  { "ssid": "ssid1", "password": ""}
]
```


### `POST /api/credentials`

**Description:**
Creates ssid and password connection

**Request Parameters:**
- `ssid` (string, required)
- `password` (string, required)

**Response:**
- `201 OK`
- `400 Bad Request`: Missing ssid
- `400 Bad Request`: Missing password

**Example Response:**
```
OK
```


### `DELETE /api/credentials`

**Description:**
Delete credential by ssid

**Request Parameters:**
- `ssid` (string, required)

**Response:**
- `200 OK`
- `400 Bad Request`: Missing ssid

**Example Response:**
```
OK
```


### `POST /api/configurations`

**Description:**
Sets the configurations

**Request Parameters:**

- `ntp_server_host` (String, required): The new volume level (0-100).
- `gmt_offset_sec` (integer, required): The new volume level (0-100).
- `daylight_offset_sec` (integer, required): The new volume level (0-100).

**Response:**
- `201 Created`: Configuration successfully updated.
- `400 Bad Request`: Missing ntp_server_host
- `400 Bad Request`: Missing gmt_offset_sec
- `400 Bad Request`: Missing daylight_offset_sec

**Example Response:**
OK