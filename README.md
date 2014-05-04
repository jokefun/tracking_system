tracking system
===============

A tracking system for stream data, including different kind of data processing
modules.

Query
-----

A query is a processing module for stream data. Queries include calculating
average, max and min for data inside a window.

Processor
---------

Processor is a wrapper for all queries, providing interfaces to add new data
into the queries and to call all defined queries at once

Processor also includes a data storage, which can be used to query for data
between a given time period.

