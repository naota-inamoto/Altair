
/* ======== message data table ======== */

create table TagPathName (
    tagId      varchar(255)   not null,
    path       varchar(1000),
    msgType    varchar(255)   not null,
    msgVersion varchar(255)   not null,
    constraint pkey_TagPathName primary key (tagId)
);
create index idx_TagPathName_msgType  on TagPathName ( msgType );
create index idx_TagPathName_version  on TagPathName ( msgVersion );

create table ParentRelation (
    dataId     varchar(255)  not null,
    msgId      varchar(255)  not null,
    msgType    varchar(255),
    msgVersion varchar(255),
    dtdFile    varchar(255),
    parentId   varchar(255)  not null,
    tagId      varchar(255)  not null,
    childSeq   varchar(255)  not null,
    fieldSeq   varchar(255)  not null,
    constraint pkey_ParentRelation primary key (dataId)
);
create index idx_ParentRelation_msgId    on ParentRelation ( msgId );
create index idx_ParentRelation_parentId on ParentRelation ( parentId );
create index idx_ParentRelation_tagId    on ParentRelation ( tagId );

create table MsgData (
    dataId     varchar(255)  not null,
    msgId      varchar(255)  not null,
    parentId   varchar(255)  not null,
    tagId      varchar(255)  not null,
    value      varchar(3000) not null,
    constraint pkey_MsgData primary key (dataId)
);
create index idx_MsgData_msgId    on MsgData ( msgId );
create index idx_MsgData_parentId on MsgData ( parentId );
create index idx_MsgData_tagId    on MsgData ( tagId );
/* create index idx_MsgData_value    on MsgData ( value ); */

/* ======== access log table ======== */

create table AccessLog (
    AccessLogId     varchar(63)  not null,
    Time            varchar(63)  not null,
    FromIP          varchar(63),
    Path            varchar(3000),
    ResponseCode    varchar(63),
    ResponseSize    varchar(63),
    constraint pkey_AccessLog primary key (AccessLogId)
);

create index idx_AccessLog_Time on AccessLog ( Time );

/* ======== system log table ======== */

create table SystemLog (
    SystemLogId  varchar(63)  not null,
    Time         varchar(63)  not null,
    LogLevel     varchar(63),
    Message      varchar(3000),
    constraint pkey_SystemLog primary key (SystemLogId)
);

create index idx_SystemLog_Time on SystemLog ( Time );

/* ======== alsoftpro log table ======== */

create table AlSoftProLog (
    AlSoftProLogId  varchar(63)  not null,
    Time            varchar(63)  not null,
    LogLevel        varchar(63),
    Message         varchar(3000),
    constraint pkey_AlSoftProLog primary key (AlSoftProLogId)
);

create index idx_AlSoftProLog_Time on AlSoftProLog ( Time );

