
/* ======== process engine table ======== */

create table ProcDef (
    ProcessDefName    varchar(63)  not null,
    constraint pkey_ProcDef primary key (ProcessDefName)
);

create table ProcessDefinition (
    ProcessDefName    varchar(63)  not null,
    ActivityId        varchar(63)  not null,
    ActivityKind      varchar(63)  not null,
    ActivityName      varchar(63),
    PropertyValue     varchar(255) not null, /* for OrSplit */
    AndJoinCount      varchar(3),   /* for AndJoin */
    Time              varchar(22),  /* wait time of wait activity */
    NextActivityId    varchar(63),
    NextActivityKind  varchar(63),
    NextActivityName  varchar(63),
    NextPropertyName  varchar(63),  /* for next OrSpplit */
    ClassName         varchar(255), /* for Automated */
    constraint pkey_ProcessDef primary key (ProcessDefName, ActivityId, PropertyValue)
);

create table ProcessDefaultProperties (
    ProcessDefName  varchar(63) not null,
    Name            varchar(63) not null,
    Value           varchar(3000),
    constraint pkey_ProcessDefaultProps primary key (ProcessDefName, Name)
);

create table ProcessQueue (
    ProcessDefName         varchar(63) not null,
    ProcessId              varchar(63) not null,
    CurrActivityId         varchar(63),
    CurrActivityKind       varchar(63),
    CurrActivityName       varchar(63),
    ActivityId             varchar(63) not null,
    ActivityKind           varchar(63) not null,
    ActivityName           varchar(63),
    PropertyName           varchar(63),
    Status                 varchar(10), /* "execute", "wait", "manual", "suspend", "cleanup" */
    Time                   varchar(22), /* wait time of wait activity */
    constraint  pkey_ProcessQueue primary key (ProcessId, ActivityId)
);

create index idx_ProcessQueue_Status on ProcessQueue ( Status );
create index idx_ProcessQueue_Time   on ProcessQueue ( Time );

create table CmdQueue (
    CmdQueueId    varchar(63) not null,
    ProcessId     varchar(63) not null,
    Command       varchar(20),   /* "putProperty", "suspend", "resume", "abort", "complete" */
    Arg1          varchar(63),   /* propertyName  ,                              activityId */
    Arg2          varchar(3000), /* propertyValue */
    constraint  pkey_CmdQueue primary key (CmdQueueId)
);

create table ProcessProperties (
    ProcessId       varchar(63) not null,
    Name            varchar(63) not null,
    Value           varchar(3000),
    constraint pkey_ProcessProps primary key (ProcessId, Name)
);

create table ProcessActivities (
    ProcessDefName    varchar(63)  not null,
    ProcessId         varchar(63)  not null,
    ActivityId        varchar(63)  not null,
    ActivityKind      varchar(63)  not null,
    ActivityName      varchar(63),
    PropertyValue     varchar(255) not null, /* for OrSplit */
    AndJoinCount      varchar(3),   /* for AndJoin */
    Time              varchar(23),  /* wait time of wait activity */
    NextActivityId    varchar(63),
    NextActivityKind  varchar(63),
    NextActivityName  varchar(63),
    NextPropertyName  varchar(63),  /* for next OrSplit */
    ClassName         varchar(255), /* for Automated */
    Status            varchar(10),  /* "deactive", "active" */
    Count             varchar(10),
    constraint pkey_ProcessActivities primary key (ProcessId, ActivityId, PropertyValue)
);

create table ProcessLog (
    ProcessLogId    varchar(63)  not null,
    Action          varchar(63),
    Time            varchar(63)  not null,
    ProcessDefName  varchar(63),
    ActivityName    varchar(63),
    ActivityKind    varchar(63),
    ProcessId       varchar(63),
    ActivityId      varchar(63),
    Message         varchar(3000),
    constraint pkey_ProcessLog primary key (ProcessLogId)
);

create index idx_ProcessLog_Action on ProcessLog ( Action );
create index idx_ProcessLog_Time   on ProcessLog ( Time );

create sequence ProcessId_seq;
create sequence CmdQueueId_seq;
create sequence ProcessLogId_seq;

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
create index idx_MsgData_value    on MsgData ( value );

create sequence tagId_seq;
create sequence msgId_seq;

create table CommLog (
    CommLogId       varchar(63) not null,
    Target          varchar(63),
    SendRecv        varchar(63),
    LogLevel        varchar(63),
    Time            varchar(63),
    MsgType         varchar(255),
    Reason          varchar(3000),
    URLfromIP       varchar(255),
    MyId            varchar(255),
    PartnerId       varchar(255),
    rawMsgId        varchar(255),
    MsgId           varchar(255),
    RefMsgId        varchar(255),
    TransactionId   varchar(255),
    Pid             varchar(255),
    AppPid          varchar(255),
    constraint pkey_CommLog primary key (CommLogId)
);

create index idx_CommLog_Time on CommLog ( Time );

create sequence CommLogId_seq;

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

create sequence AccessLogId_seq;

/* ======== proxy log table ======== */

create table ProxyLog (
    ProxyLogId      varchar(63)  not null,
    Time            varchar(63)  not null,
    FromIP          varchar(63),
    Path            varchar(3000),
    Reason          varchar(3000),
    constraint pkey_ProxyLog primary key (ProxyLogId)
);

create index idx_ProxyLog_Time on ProxyLog ( Time );

create sequence ProxyLogId_seq;

/* ======== system log table ======== */

create table SystemLog (
    SystemLogId     varchar(63)  not null,
    Time         varchar(63)  not null,
    LogLevel     varchar(63),
    Message      varchar(3000),
    constraint pkey_SystemLog primary key (SystemLogId)
);

create index idx_SystemLog_Time on SystemLog ( Time );

create sequence SystemLogId_seq;

