.. title:: clang-tidy - mesos-unused-return-value

mesos-unused-return-value
==================

In Mesos exceptions are not used; instead functions return union types like
`Try` or `Result` and explicitly handle possible failure state. In this pattern
failure states are explicit in the type system (via the function signature),
but to be effective callers also need to handle function return values.

The following example is either broken or brittle,

    Try<Nothing> createFile(const Path&);

    // Broken as code might continue even though file might not exist.
    createFile("/foo/bar");

and should instead explicitly handle failure state, e.g.,

    Try<Nothing> created = createFile("/foo/bar");

    if (created.isError()) { return Error(created.error()); }

This check by default checks return values for functions returning `Try` and
`Result`. An alternative list of types can be configured via the checker's
`Types` config parameter.
