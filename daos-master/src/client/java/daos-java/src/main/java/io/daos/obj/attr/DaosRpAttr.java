// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: DaosObjectAttribute.proto

package io.daos.obj.attr;

/**
 * Protobuf type {@code objattr.DaosRpAttr}
 */
public final class DaosRpAttr extends
    com.google.protobuf.GeneratedMessageV3 implements
    // @@protoc_insertion_point(message_implements:objattr.DaosRpAttr)
    DaosRpAttrOrBuilder {
private static final long serialVersionUID = 0L;
  // Use DaosRpAttr.newBuilder() to construct.
  private DaosRpAttr(com.google.protobuf.GeneratedMessageV3.Builder<?> builder) {
    super(builder);
  }
  private DaosRpAttr() {
  }

  @java.lang.Override
  @SuppressWarnings({"unused"})
  protected java.lang.Object newInstance(
      UnusedPrivateParameter unused) {
    return new DaosRpAttr();
  }

  @java.lang.Override
  public final com.google.protobuf.UnknownFieldSet
  getUnknownFields() {
    return this.unknownFields;
  }
  private DaosRpAttr(
      com.google.protobuf.CodedInputStream input,
      com.google.protobuf.ExtensionRegistryLite extensionRegistry)
      throws com.google.protobuf.InvalidProtocolBufferException {
    this();
    if (extensionRegistry == null) {
      throw new java.lang.NullPointerException();
    }
    com.google.protobuf.UnknownFieldSet.Builder unknownFields =
        com.google.protobuf.UnknownFieldSet.newBuilder();
    try {
      boolean done = false;
      while (!done) {
        int tag = input.readTag();
        switch (tag) {
          case 0:
            done = true;
            break;
          case 8: {

            rProto_ = input.readUInt32();
            break;
          }
          case 16: {

            rNum_ = input.readUInt32();
            break;
          }
          default: {
            if (!parseUnknownField(
                input, unknownFields, extensionRegistry, tag)) {
              done = true;
            }
            break;
          }
        }
      }
    } catch (com.google.protobuf.InvalidProtocolBufferException e) {
      throw e.setUnfinishedMessage(this);
    } catch (java.io.IOException e) {
      throw new com.google.protobuf.InvalidProtocolBufferException(
          e).setUnfinishedMessage(this);
    } finally {
      this.unknownFields = unknownFields.build();
      makeExtensionsImmutable();
    }
  }
  public static final com.google.protobuf.Descriptors.Descriptor
      getDescriptor() {
    return io.daos.obj.attr.DaosObjAttrClasses.internal_static_objattr_DaosRpAttr_descriptor;
  }

  @java.lang.Override
  protected com.google.protobuf.GeneratedMessageV3.FieldAccessorTable
      internalGetFieldAccessorTable() {
    return io.daos.obj.attr.DaosObjAttrClasses.internal_static_objattr_DaosRpAttr_fieldAccessorTable
        .ensureFieldAccessorsInitialized(
            io.daos.obj.attr.DaosRpAttr.class, io.daos.obj.attr.DaosRpAttr.Builder.class);
  }

  public static final int R_PROTO_FIELD_NUMBER = 1;
  private int rProto_;
  /**
   * <code>uint32 r_proto = 1;</code>
   * @return The rProto.
   */
  @java.lang.Override
  public int getRProto() {
    return rProto_;
  }

  public static final int R_NUM_FIELD_NUMBER = 2;
  private int rNum_;
  /**
   * <code>uint32 r_num = 2;</code>
   * @return The rNum.
   */
  @java.lang.Override
  public int getRNum() {
    return rNum_;
  }

  private byte memoizedIsInitialized = -1;
  @java.lang.Override
  public final boolean isInitialized() {
    byte isInitialized = memoizedIsInitialized;
    if (isInitialized == 1) return true;
    if (isInitialized == 0) return false;

    memoizedIsInitialized = 1;
    return true;
  }

  @java.lang.Override
  public void writeTo(com.google.protobuf.CodedOutputStream output)
                      throws java.io.IOException {
    if (rProto_ != 0) {
      output.writeUInt32(1, rProto_);
    }
    if (rNum_ != 0) {
      output.writeUInt32(2, rNum_);
    }
    unknownFields.writeTo(output);
  }

  @java.lang.Override
  public int getSerializedSize() {
    int size = memoizedSize;
    if (size != -1) return size;

    size = 0;
    if (rProto_ != 0) {
      size += com.google.protobuf.CodedOutputStream
        .computeUInt32Size(1, rProto_);
    }
    if (rNum_ != 0) {
      size += com.google.protobuf.CodedOutputStream
        .computeUInt32Size(2, rNum_);
    }
    size += unknownFields.getSerializedSize();
    memoizedSize = size;
    return size;
  }

  @java.lang.Override
  public boolean equals(final java.lang.Object obj) {
    if (obj == this) {
     return true;
    }
    if (!(obj instanceof io.daos.obj.attr.DaosRpAttr)) {
      return super.equals(obj);
    }
    io.daos.obj.attr.DaosRpAttr other = (io.daos.obj.attr.DaosRpAttr) obj;

    if (getRProto()
        != other.getRProto()) return false;
    if (getRNum()
        != other.getRNum()) return false;
    if (!unknownFields.equals(other.unknownFields)) return false;
    return true;
  }

  @java.lang.Override
  public int hashCode() {
    if (memoizedHashCode != 0) {
      return memoizedHashCode;
    }
    int hash = 41;
    hash = (19 * hash) + getDescriptor().hashCode();
    hash = (37 * hash) + R_PROTO_FIELD_NUMBER;
    hash = (53 * hash) + getRProto();
    hash = (37 * hash) + R_NUM_FIELD_NUMBER;
    hash = (53 * hash) + getRNum();
    hash = (29 * hash) + unknownFields.hashCode();
    memoizedHashCode = hash;
    return hash;
  }

  public static io.daos.obj.attr.DaosRpAttr parseFrom(
      java.nio.ByteBuffer data)
      throws com.google.protobuf.InvalidProtocolBufferException {
    return PARSER.parseFrom(data);
  }
  public static io.daos.obj.attr.DaosRpAttr parseFrom(
      java.nio.ByteBuffer data,
      com.google.protobuf.ExtensionRegistryLite extensionRegistry)
      throws com.google.protobuf.InvalidProtocolBufferException {
    return PARSER.parseFrom(data, extensionRegistry);
  }
  public static io.daos.obj.attr.DaosRpAttr parseFrom(
      com.google.protobuf.ByteString data)
      throws com.google.protobuf.InvalidProtocolBufferException {
    return PARSER.parseFrom(data);
  }
  public static io.daos.obj.attr.DaosRpAttr parseFrom(
      com.google.protobuf.ByteString data,
      com.google.protobuf.ExtensionRegistryLite extensionRegistry)
      throws com.google.protobuf.InvalidProtocolBufferException {
    return PARSER.parseFrom(data, extensionRegistry);
  }
  public static io.daos.obj.attr.DaosRpAttr parseFrom(byte[] data)
      throws com.google.protobuf.InvalidProtocolBufferException {
    return PARSER.parseFrom(data);
  }
  public static io.daos.obj.attr.DaosRpAttr parseFrom(
      byte[] data,
      com.google.protobuf.ExtensionRegistryLite extensionRegistry)
      throws com.google.protobuf.InvalidProtocolBufferException {
    return PARSER.parseFrom(data, extensionRegistry);
  }
  public static io.daos.obj.attr.DaosRpAttr parseFrom(java.io.InputStream input)
      throws java.io.IOException {
    return com.google.protobuf.GeneratedMessageV3
        .parseWithIOException(PARSER, input);
  }
  public static io.daos.obj.attr.DaosRpAttr parseFrom(
      java.io.InputStream input,
      com.google.protobuf.ExtensionRegistryLite extensionRegistry)
      throws java.io.IOException {
    return com.google.protobuf.GeneratedMessageV3
        .parseWithIOException(PARSER, input, extensionRegistry);
  }
  public static io.daos.obj.attr.DaosRpAttr parseDelimitedFrom(java.io.InputStream input)
      throws java.io.IOException {
    return com.google.protobuf.GeneratedMessageV3
        .parseDelimitedWithIOException(PARSER, input);
  }
  public static io.daos.obj.attr.DaosRpAttr parseDelimitedFrom(
      java.io.InputStream input,
      com.google.protobuf.ExtensionRegistryLite extensionRegistry)
      throws java.io.IOException {
    return com.google.protobuf.GeneratedMessageV3
        .parseDelimitedWithIOException(PARSER, input, extensionRegistry);
  }
  public static io.daos.obj.attr.DaosRpAttr parseFrom(
      com.google.protobuf.CodedInputStream input)
      throws java.io.IOException {
    return com.google.protobuf.GeneratedMessageV3
        .parseWithIOException(PARSER, input);
  }
  public static io.daos.obj.attr.DaosRpAttr parseFrom(
      com.google.protobuf.CodedInputStream input,
      com.google.protobuf.ExtensionRegistryLite extensionRegistry)
      throws java.io.IOException {
    return com.google.protobuf.GeneratedMessageV3
        .parseWithIOException(PARSER, input, extensionRegistry);
  }

  @java.lang.Override
  public Builder newBuilderForType() { return newBuilder(); }
  public static Builder newBuilder() {
    return DEFAULT_INSTANCE.toBuilder();
  }
  public static Builder newBuilder(io.daos.obj.attr.DaosRpAttr prototype) {
    return DEFAULT_INSTANCE.toBuilder().mergeFrom(prototype);
  }
  @java.lang.Override
  public Builder toBuilder() {
    return this == DEFAULT_INSTANCE
        ? new Builder() : new Builder().mergeFrom(this);
  }

  @java.lang.Override
  protected Builder newBuilderForType(
      com.google.protobuf.GeneratedMessageV3.BuilderParent parent) {
    Builder builder = new Builder(parent);
    return builder;
  }
  /**
   * Protobuf type {@code objattr.DaosRpAttr}
   */
  public static final class Builder extends
      com.google.protobuf.GeneratedMessageV3.Builder<Builder> implements
      // @@protoc_insertion_point(builder_implements:objattr.DaosRpAttr)
      io.daos.obj.attr.DaosRpAttrOrBuilder {
    public static final com.google.protobuf.Descriptors.Descriptor
        getDescriptor() {
      return io.daos.obj.attr.DaosObjAttrClasses.internal_static_objattr_DaosRpAttr_descriptor;
    }

    @java.lang.Override
    protected com.google.protobuf.GeneratedMessageV3.FieldAccessorTable
        internalGetFieldAccessorTable() {
      return io.daos.obj.attr.DaosObjAttrClasses.internal_static_objattr_DaosRpAttr_fieldAccessorTable
          .ensureFieldAccessorsInitialized(
              io.daos.obj.attr.DaosRpAttr.class, io.daos.obj.attr.DaosRpAttr.Builder.class);
    }

    // Construct using io.daos.obj.attr.DaosRpAttr.newBuilder()
    private Builder() {
      maybeForceBuilderInitialization();
    }

    private Builder(
        com.google.protobuf.GeneratedMessageV3.BuilderParent parent) {
      super(parent);
      maybeForceBuilderInitialization();
    }
    private void maybeForceBuilderInitialization() {
      if (com.google.protobuf.GeneratedMessageV3
              .alwaysUseFieldBuilders) {
      }
    }
    @java.lang.Override
    public Builder clear() {
      super.clear();
      rProto_ = 0;

      rNum_ = 0;

      return this;
    }

    @java.lang.Override
    public com.google.protobuf.Descriptors.Descriptor
        getDescriptorForType() {
      return io.daos.obj.attr.DaosObjAttrClasses.internal_static_objattr_DaosRpAttr_descriptor;
    }

    @java.lang.Override
    public io.daos.obj.attr.DaosRpAttr getDefaultInstanceForType() {
      return io.daos.obj.attr.DaosRpAttr.getDefaultInstance();
    }

    @java.lang.Override
    public io.daos.obj.attr.DaosRpAttr build() {
      io.daos.obj.attr.DaosRpAttr result = buildPartial();
      if (!result.isInitialized()) {
        throw newUninitializedMessageException(result);
      }
      return result;
    }

    @java.lang.Override
    public io.daos.obj.attr.DaosRpAttr buildPartial() {
      io.daos.obj.attr.DaosRpAttr result = new io.daos.obj.attr.DaosRpAttr(this);
      result.rProto_ = rProto_;
      result.rNum_ = rNum_;
      onBuilt();
      return result;
    }

    @java.lang.Override
    public Builder clone() {
      return super.clone();
    }
    @java.lang.Override
    public Builder setField(
        com.google.protobuf.Descriptors.FieldDescriptor field,
        java.lang.Object value) {
      return super.setField(field, value);
    }
    @java.lang.Override
    public Builder clearField(
        com.google.protobuf.Descriptors.FieldDescriptor field) {
      return super.clearField(field);
    }
    @java.lang.Override
    public Builder clearOneof(
        com.google.protobuf.Descriptors.OneofDescriptor oneof) {
      return super.clearOneof(oneof);
    }
    @java.lang.Override
    public Builder setRepeatedField(
        com.google.protobuf.Descriptors.FieldDescriptor field,
        int index, java.lang.Object value) {
      return super.setRepeatedField(field, index, value);
    }
    @java.lang.Override
    public Builder addRepeatedField(
        com.google.protobuf.Descriptors.FieldDescriptor field,
        java.lang.Object value) {
      return super.addRepeatedField(field, value);
    }
    @java.lang.Override
    public Builder mergeFrom(com.google.protobuf.Message other) {
      if (other instanceof io.daos.obj.attr.DaosRpAttr) {
        return mergeFrom((io.daos.obj.attr.DaosRpAttr)other);
      } else {
        super.mergeFrom(other);
        return this;
      }
    }

    public Builder mergeFrom(io.daos.obj.attr.DaosRpAttr other) {
      if (other == io.daos.obj.attr.DaosRpAttr.getDefaultInstance()) return this;
      if (other.getRProto() != 0) {
        setRProto(other.getRProto());
      }
      if (other.getRNum() != 0) {
        setRNum(other.getRNum());
      }
      this.mergeUnknownFields(other.unknownFields);
      onChanged();
      return this;
    }

    @java.lang.Override
    public final boolean isInitialized() {
      return true;
    }

    @java.lang.Override
    public Builder mergeFrom(
        com.google.protobuf.CodedInputStream input,
        com.google.protobuf.ExtensionRegistryLite extensionRegistry)
        throws java.io.IOException {
      io.daos.obj.attr.DaosRpAttr parsedMessage = null;
      try {
        parsedMessage = PARSER.parsePartialFrom(input, extensionRegistry);
      } catch (com.google.protobuf.InvalidProtocolBufferException e) {
        parsedMessage = (io.daos.obj.attr.DaosRpAttr) e.getUnfinishedMessage();
        throw e.unwrapIOException();
      } finally {
        if (parsedMessage != null) {
          mergeFrom(parsedMessage);
        }
      }
      return this;
    }

    private int rProto_ ;
    /**
     * <code>uint32 r_proto = 1;</code>
     * @return The rProto.
     */
    @java.lang.Override
    public int getRProto() {
      return rProto_;
    }
    /**
     * <code>uint32 r_proto = 1;</code>
     * @param value The rProto to set.
     * @return This builder for chaining.
     */
    public Builder setRProto(int value) {
      
      rProto_ = value;
      onChanged();
      return this;
    }
    /**
     * <code>uint32 r_proto = 1;</code>
     * @return This builder for chaining.
     */
    public Builder clearRProto() {
      
      rProto_ = 0;
      onChanged();
      return this;
    }

    private int rNum_ ;
    /**
     * <code>uint32 r_num = 2;</code>
     * @return The rNum.
     */
    @java.lang.Override
    public int getRNum() {
      return rNum_;
    }
    /**
     * <code>uint32 r_num = 2;</code>
     * @param value The rNum to set.
     * @return This builder for chaining.
     */
    public Builder setRNum(int value) {
      
      rNum_ = value;
      onChanged();
      return this;
    }
    /**
     * <code>uint32 r_num = 2;</code>
     * @return This builder for chaining.
     */
    public Builder clearRNum() {
      
      rNum_ = 0;
      onChanged();
      return this;
    }
    @java.lang.Override
    public final Builder setUnknownFields(
        final com.google.protobuf.UnknownFieldSet unknownFields) {
      return super.setUnknownFields(unknownFields);
    }

    @java.lang.Override
    public final Builder mergeUnknownFields(
        final com.google.protobuf.UnknownFieldSet unknownFields) {
      return super.mergeUnknownFields(unknownFields);
    }


    // @@protoc_insertion_point(builder_scope:objattr.DaosRpAttr)
  }

  // @@protoc_insertion_point(class_scope:objattr.DaosRpAttr)
  private static final io.daos.obj.attr.DaosRpAttr DEFAULT_INSTANCE;
  static {
    DEFAULT_INSTANCE = new io.daos.obj.attr.DaosRpAttr();
  }

  public static io.daos.obj.attr.DaosRpAttr getDefaultInstance() {
    return DEFAULT_INSTANCE;
  }

  private static final com.google.protobuf.Parser<DaosRpAttr>
      PARSER = new com.google.protobuf.AbstractParser<DaosRpAttr>() {
    @java.lang.Override
    public DaosRpAttr parsePartialFrom(
        com.google.protobuf.CodedInputStream input,
        com.google.protobuf.ExtensionRegistryLite extensionRegistry)
        throws com.google.protobuf.InvalidProtocolBufferException {
      return new DaosRpAttr(input, extensionRegistry);
    }
  };

  public static com.google.protobuf.Parser<DaosRpAttr> parser() {
    return PARSER;
  }

  @java.lang.Override
  public com.google.protobuf.Parser<DaosRpAttr> getParserForType() {
    return PARSER;
  }

  @java.lang.Override
  public io.daos.obj.attr.DaosRpAttr getDefaultInstanceForType() {
    return DEFAULT_INSTANCE;
  }

}

